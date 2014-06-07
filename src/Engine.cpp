
/*******************************************************************
 * Voxigine - A simple voxel engine
 * Copyright(C) 2014 Simon Judd
 *
 * Email:       sirjuddington@gmail.com
 * Web:         https://github.com/sirjuddington/Voxigine
 * Filename:    Engine.cpp
 * Description: The main engine, handles startup/shutdown and the
 *              main loop
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *******************************************************************/


/*******************************************************************
 * INCLUDES
 *******************************************************************/
#include "Main.h"
#include "Engine.h"
#include "Console.h"
#include "Utilities/Tokenizer.h"
#include "glew/glew.h"
#include <SFML/Graphics.hpp>
#include <fstream>

#include "Shader.h"


/*******************************************************************
 * VARIABLES
 *******************************************************************/
namespace Engine
{
	sf::RenderWindow*	window = nullptr;
	std::ofstream		log;
	sf::Clock			clock;
	sf::Time			last_time;

	// Testing
	Shader* shader_vertex = nullptr;
	Shader* shader_fragment = nullptr;
}
CVAR(Int, vid_win_width, 1024, CVAR_SAVE|CVAR_LOCKED)
CVAR(Int, vid_win_height, 768, CVAR_SAVE|CVAR_LOCKED)
CVAR(Int, vid_fs_width, -1, CVAR_SAVE|CVAR_LOCKED)
CVAR(Int, vid_fs_height, -1, CVAR_SAVE|CVAR_LOCKED)
CVAR(Bool, vid_fullscreen, false, CVAR_SAVE|CVAR_LOCKED)
CVAR(Int, vid_aa_level, 0, CVAR_SAVE)
CVAR(Bool, vid_vsync, true, CVAR_SAVE)


/*******************************************************************
 * ENGINE NAMESPACE FUNCTIONS
 *******************************************************************/

/* Engine::init
 * Initialises the game engine, returns true on success
 *******************************************************************/
bool Engine::init()
{
	// Redirect sf::err output to a log file
	log.open("voxigine.log");
	std::streambuf* previous = sf::err().rdbuf(log.rdbuf());
	logMessage(1, "Voxigine v0.1 alpha");
	logMessage(1, "(c) 2014 Simon Judd");
	logMessage(1, "-------------------");

	// Load configuration
	if (!loadConfig())
		logMessage(1, "Unable to open voxigine.cfg, will be created on exit");

	createWindow();

	// Init GLEW
	glewInit();

	// Test
	shader_vertex = new Shader(GL_VERTEX_SHADER);
	shader_fragment = new Shader(GL_FRAGMENT_SHADER);
	shader_vertex->openFile("shaders/vertex.glsl");
	shader_fragment->openFile("shaders/fragment.glsl");
	ShaderProgram program;
	program.addShader(shader_vertex);
	program.addShader(shader_fragment);
	program.link();

	return true;
}

/* Engine::createWindow
 * Creates the window based on current settings
 *******************************************************************/
bool Engine::createWindow()
{
	// Close current window if needed
	if (window)
	{
		window->close();
		delete window;
		window = nullptr;
	}

	// Setup resolution if needed
	if (vid_fs_width < 0)
	{
		sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
		vid_fs_width = desktop.width;
		vid_fs_height = desktop.height;
		logMessage(1, "Set fullscreen resolution to desktop: %dx%d", desktop.width, desktop.height);
	}

	// Create main window
	sf::ContextSettings settings;
	settings.majorVersion = 3;
	settings.minorVersion = 0;
	settings.antialiasingLevel = vid_aa_level;
	if (vid_fullscreen)
		window = new sf::RenderWindow(sf::VideoMode(vid_fs_width, vid_fs_height), "Voxigine", sf::Style::Fullscreen, settings);
	else
		window = new sf::RenderWindow(sf::VideoMode(vid_win_width, vid_win_height), "Voxigine", sf::Style::Default, settings);
	window->setVerticalSyncEnabled(vid_vsync);

	logMessage(1, "Set video mode to: %dx%d (%s)", window->getSize().x, window->getSize().y, vid_fullscreen ? "fullscreen" : "windowed");
	logMessage(1, "Antialiasing level: %dx", window->getSettings().antialiasingLevel);

	return window->isOpen();
}

/* Engine::loadConfig
 * Reads the Voxigine config file
 *******************************************************************/
bool Engine::loadConfig()
{
	Tokenizer tz;
	if (!tz.openFile("voxigine.cfg"))
		return false;

	string token = tz.getToken();
	while (!token.empty())
	{
		// CVars section
		if (token == "cvars")
		{
			token = tz.getToken();	// Skip '{'

			// Keep reading name/value pairs until we hit the ending '}'
			string cvar_name = tz.getToken();
			while (cvar_name != "}")
			{
				string cvar_val = tz.getToken();
				read_cvar(cvar_name, cvar_val);
				cvar_name = tz.getToken();
			}
		}

		// Get next token
		token = tz.getToken();
	}
	
	return true;
}

/* Engine::saveConfig
 * Writes the Voxigine config file
 *******************************************************************/
void Engine::saveConfig()
{
	// Open config file
	FILE* fp = fopen("voxigine.cfg", "wt");
	if (!fp)
		return;

	// Write cvars
	save_cvars(fp);

	// Close file
	fclose(fp);
}

/* Engine::mainLoop
 * The main engine loop, returns false on exit
 *******************************************************************/
bool Engine::mainLoop()
{
	sf::Event event;
	while (window->pollEvent(event))
	{
		// Close window
		if (event.type == sf::Event::Closed)
		{
			window->close();
			return false;
		}

		// Resize window
		else if (event.type == sf::Event::Resized)
			window->setView(sf::View(sf::FloatRect(0.0f, 0.0f, event.size.width, event.size.height)));

		else if (event.type == sf::Event::KeyPressed)
		{
			// Console input
			if (Console::isActive())
				Console::handleKeyPress(event.key);

			// Tilde (open console)
			if (event.key.code == sf::Keyboard::Tilde)
				Console::activate(!Console::isActive());

			// Escape (exit)
			if (event.key.code == sf::Keyboard::Escape)
			{
				window->close();
				return false;
			}
		}

		else if (event.type == sf::Event::TextEntered)
			Console::handleText(event.text);
	}

	window->clear(sf::Color::Black);

	Console::draw(window);

	int ms = clock.getElapsedTime().asMilliseconds() - last_time.asMilliseconds();
	last_time = clock.getElapsedTime();

	window->display();

	return true;
}

/* Engine::shutDown
 * Shuts down the game engine, performs cleanup etc
 *******************************************************************/
void Engine::shutDown()
{
	logMessage(1, "Exiting...");
	delete shader_vertex;
	delete shader_fragment;

	saveConfig();

	log.close();
}

/* Engine::resizeWindow
 * Resizes the window to [width]x[height] (windowed mode only)
 *******************************************************************/
void Engine::resizeWindow(int width, int height)
{
	// Ignore if fullscreen
	if (vid_fullscreen)
		return;

	window->setSize(sf::Vector2u(width, height));
	logMessage(1, "Set video mode to: %dx%d (%s)", window->getSize().x, window->getSize().y, vid_fullscreen ? "fullscreen" : "windowed");
}


/*******************************************************************
 * CONSOLE COMMANDS
 *******************************************************************/

/* vid_setmode
 * Sets the video mode
 *******************************************************************/
CONSOLE_COMMAND(vid_setmode, 2, true)
{
	// Set fullscreen
	bool was_fullscreen = vid_fullscreen;
	if (args.size() > 2)
	{
		int fullscreen = atoi(args[2].c_str());
		vid_fullscreen = (fullscreen > 0);
	}

	// Set resolution
	int width = atoi(args[0].c_str());
	int height = atoi(args[1].c_str());
	if (vid_fullscreen)
	{
		vid_fs_width = width;
		vid_fs_height = height;
	}
	else
	{
		vid_win_width = width;
		vid_win_height = height;
	}

	// Recreate window
	if (!was_fullscreen && !vid_fullscreen)
		Engine::resizeWindow(width, height);
	else
		Engine::createWindow();
}

/* fullscreen
 * Sets fullscreen or windowed mode
 *******************************************************************/
CONSOLE_COMMAND(fullscreen, 1, true)
{
	bool was_fullscreen = vid_fullscreen;
	int fullscreen = atoi(args[0].c_str());
	vid_fullscreen = (fullscreen > 0);

	if (was_fullscreen != vid_fullscreen)
		Engine::createWindow();
}
