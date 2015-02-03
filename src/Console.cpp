
/*******************************************************************
 * Voxigine - A simple voxel engine
 * Copyright(C) 2014 Simon Judd
 *
 * Email:       sirjuddington@gmail.com
 * Web:         https://github.com/sirjuddington/Voxigine
 * Filename:    Console.cpp
 * Description: The Voxigine console
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
#include "Console.h"
#include "Utilities/Tokenizer.h"
#include <SFML/Graphics.hpp>


/*******************************************************************
 * VARIABLES
 *******************************************************************/
namespace Console
{
	vector<string>	log;

	vector<Command>	commands;
	string			cmd_current;
	vector<string>	cmd_log;
	int				cmd_log_current = -1;

	sf::Font		console_font;
	bool			console_font_loaded = false;

	int				caret_position = 0;
	sf::Clock		caret_ticker;

	bool			active = true;
	sf::Clock		activate_anim;
}
CVAR(Int, console_show_lines, 25, CVAR_SAVE)
CVAR(Int, console_font_size, 16, CVAR_SAVE)


/*******************************************************************
 * CONSOLE NAMESPACE FUNCTIONS
 *******************************************************************/

/* Console::numCommands
 * Returns the number of defined console commands
 *******************************************************************/
unsigned Console::numCommands()
{
	return commands.size();
}

/* Console::command
 * Returns the ConsoleCommand at the specified index
 *******************************************************************/
Console::Command& Console::command(size_t index)
{
	if (index < commands.size())
		return commands[index];
	else
		return commands[0]; // Return first console command on invalid index
}

/* Console::addCommand
 * Adds a Command to the Console
 *******************************************************************/
void Console::addCommand(Console::Command& c)
{
	// Add the command to the list
	commands.push_back(c);

	// Sort the commands alphabetically by name (so the cmdlist command output looks nice :P)
	sort(commands.begin(), commands.end());
}

/* Console::execute
 * Attempts to execute the command line given
 *******************************************************************/
void Console::execute(string command)
{
	::logMessage(1, "> %s", CHR(command));

	// Don't bother doing anything else with an empty command
	if (command.size() == 0)
		return;

	// Add the command to the log
	cmd_log.insert(cmd_log.begin(), command);

	// Tokenize the command string
	Tokenizer tz;
	tz.openString(command);

	// Get the command name
	string cmd_name = tz.getToken();

	// Get all args
	string arg = tz.getToken();
	vector<string> args;
	while (arg != "")
	{
		args.push_back(arg);
		arg = tz.getToken();
	}

	// Check that it is a valid command
	//for (size_t a = 0; a < commands.size(); a++)
	for (Command& command : commands)
	{
		// Found it, execute and return
		if (command.getName() == cmd_name)
		{
			command.execute(args);
			return;
		}
	}

	// Check if it is a cvar
	CVar* cvar = get_cvar(cmd_name);
	if (cvar)
	{
		// Arg(s) given, set cvar value
		if (args.size() > 0)
		{
			if ((cvar->flags & CVAR_LOCKED) > 0)
				logMessage(S_FMT("CVar \"%s\" can not be modified via the console", CHR(cmd_name)));
			else if (cvar->type == CVAR_BOOLEAN)
			{
				if (args[0] == "0" || args[0] == "false")
					*((CBoolCVar*)cvar) = false;
				else
					*((CBoolCVar*)cvar) = true;
			}
			else if (cvar->type == CVAR_INTEGER)
				*((CIntCVar*)cvar) = atoi(CHR(args[0]));
			else if (cvar->type == CVAR_FLOAT)
				*((CFloatCVar*)cvar) = (float)atof(CHR(args[0]));
			else if (cvar->type == CVAR_STRING)
				*((CStringCVar*)cvar) = args[0];
		}

		// Print cvar value
		string value = "";
		if (cvar->type == CVAR_BOOLEAN)
		{
			if (cvar->GetValue().Bool)
				value = "true";
			else
				value = "false";
		}
		else if (cvar->type == CVAR_INTEGER)
			value = S_FMT("%d", cvar->GetValue().Int);
		else if (cvar->type == CVAR_FLOAT)
			value = S_FMT("%1.4f", cvar->GetValue().Float);
		else
			value = ((CStringCVar*)cvar)->value;

		logMessage(S_FMT("\"%s\" = \"%s\"", CHR(cmd_name), CHR(value)));

		if (cmd_name == "log_verbosity")
			Global::log_verbosity = cvar->GetValue().Int;

		return;
	}

	// Toggle global debug mode
	if (cmd_name == "debug")
	{
		Global::debug = !Global::debug;
		if (Global::debug)
			logMessage("Debugging stuff enabled");
		else
			logMessage("Debugging stuff disabled");

		return;
	}

	// Command not found
	logMessage(S_FMT("Unknown command: \"%s\"", CHR(cmd_name)));
	return;
}

/* Console::logMessage
 * Prints a message to the console log
 *******************************************************************/
void Console::logMessage(string message)
{
	// Add a newline to the end of the message if there isn't one
	if (message.size() == 0 || message.back() != '\n')
		message += "\n";

	// Log the message
	log.push_back(message);
}

/* Console::lastLogLine
 * Returns the last line added to the console log
 *******************************************************************/
string Console::lastLogLine()
{
	// Init blank string
	string lastLine = "";

	// Get last line if any exist
	if (log.size() > 0)
		lastLine = log.back();

	return lastLine;
}

/* Console::lastCommand
 * Returns the last command sent to the console
 *******************************************************************/
string Console::lastCommand()
{
	// Init blank string
	string lastCmd = "";

	// Get last command if any exist
	if (cmd_log.size() > 0)
		lastCmd = cmd_log.back();

	return lastCmd;
}

/* Console::dumpLog
 * Returns the entire console log as one string, each message
 * separated by a newline
 *******************************************************************/
string Console::dumpLog()
{
	string ret = "";

	for (string line : log)
		ret += line;

	return ret;
}

/* Console::prevCommand
 * Returns the previous command at [index] from the last entered (ie,
 * index=0 will be the directly previous command)
 *******************************************************************/
string Console::prevCommand(int index)
{
	// Check index
	if (index < 0 || (unsigned)index >= cmd_log.size())
		return "";

	return cmd_log[index];
}

/* Console::numPrevCommands
 * Returns the number of previously entered commands
 *******************************************************************/
unsigned Console::numPrevCommands()
{
	return cmd_log.size();
}

/* Console::isActive
 * Returns true if the console is currently active
 *******************************************************************/
bool Console::isActive()
{
	return active;
}

/* Console::activate
 * Activates/deactivates the console
 *******************************************************************/
void Console::activate(bool enable)
{
	active = enable;
	activate_anim.restart();
}

/* Console::handleKeyPress
 * Handles a keypress event from the window, returns true if handled
 *******************************************************************/
bool Console::handleKeyPress(sf::Event::KeyEvent& event)
{
	if (!active)
		return false;

	// Tilde
	if (event.code == sf::Keyboard::Tilde)
		activate_anim.restart();

	// Backspace
	else if (event.code == sf::Keyboard::BackSpace && caret_position > 0)
	{
		if (caret_position >= (int)cmd_current.length())
			cmd_current.pop_back();
		else
			cmd_current.erase(cmd_current.begin() + caret_position - 1);

		caret_position--;
	}

	// Enter
	else if (event.code == sf::Keyboard::Return && !cmd_current.empty())
	{
		execute(cmd_current);
		cmd_current.clear();
		cmd_log_current = -1;
		caret_position = 0;
	}

	// Up
	else if (event.code == sf::Keyboard::Up && !cmd_log.empty())
	{
		cmd_log_current++;
		if (cmd_log_current >= (int)cmd_log.size())
			cmd_log_current = cmd_log.size() - 1;
		cmd_current = cmd_log[cmd_log_current];
		caret_position = cmd_current.length();
	}

	// Down
	else if (event.code == sf::Keyboard::Down && !cmd_log.empty())
	{
		cmd_current.clear();
		cmd_log_current--;
		if (cmd_log_current <= -1)
			cmd_log_current = -1;
		else
			cmd_current = cmd_log[cmd_log_current];
		caret_position = cmd_current.length();
	}

	// Left
	else if (event.code == sf::Keyboard::Left)
	{
		if (caret_position > 0)
			caret_position--;
	}

	// Right
	else if (event.code == sf::Keyboard::Right)
	{
		if (caret_position < (int)cmd_current.length())
			caret_position++;
	}

	// Unhandled
	else
		return false;

	return true;
}

/* Console::handleText
 * Handles a text keypress event from the window, returns true if
 * handled
 *******************************************************************/
bool Console::handleText(sf::Event::TextEvent& event)
{
	if (!active)
		return false;

	if (event.unicode > 31 && event.unicode < 0x80 && event.unicode != '`')
	{
		//cmd_current += event.unicode;
		if (caret_position >= (int)cmd_current.length())
			cmd_current += event.unicode;
		else
			cmd_current.insert(cmd_current.begin() + caret_position, event.unicode);
		caret_position++;

		return true;
	}

	return false;
}

/* Console::draw
 * Draws the console to [window]
 *******************************************************************/
void Console::draw(sf::RenderWindow* window)
{
	int32_t anim_ms = activate_anim.getElapsedTime().asMilliseconds();
	if (!active && anim_ms >= 300)
		return;

	// Load font if needed
	int line_height = 16;
	int font_size = console_font_size;
	if (!console_font_loaded)
	{
		if (console_font.loadFromFile("fonts/console.ttf"))
			console_font_loaded = true;
	}
	if (console_font_loaded)
		line_height = console_font.getLineSpacing(font_size) + 1;

	// Determine positions
	float slide_anim = 1.0f;
	if (active && anim_ms < 300)
		slide_anim = (float)anim_ms / 300;
	else if (!active && anim_ms < 300)
		slide_anim = float(300 - anim_ms) / 300;
	float console_height = (line_height * (console_show_lines + 1)) * slide_anim;

	// Draw background
	sf::RectangleShape background;
	background.setPosition(-4.0f, -4.0f);
	background.setSize(sf::Vector2f(window->getSize().x + 8.0f, console_height + 4.0f));
	background.setFillColor(sf::Color(0, 100, 255, uint8_t(200 * slide_anim)));
	background.setOutlineThickness(2.0f);
	background.setOutlineColor(sf::Color::White);
	window->draw(background);

	// Draw text
	if (console_font_loaded)
	{
		float pos = console_height - (line_height * 2) - 2.0f;
		sf::Text line;
		line.setCharacterSize(font_size);
		line.setFont(console_font);
		line.setColor(sf::Color(255, 255, 255, uint8_t(255 * slide_anim)));
		
		// Console log
		for (long a = log.size() - 1; a >= 0; a--)
		{
			line.setPosition(4.0f, pos);
			line.setString(log[a]);
			window->draw(line);
			pos -= line_height;
			if (pos < -line_height)
				break;
		}

		// Command line
		line.setPosition(4.0f, console_height - line_height - 2.0f);
		line.setString("> " + cmd_current);
		window->draw(line);

		// Caret
		if (caret_ticker.getElapsedTime().asMilliseconds() > 600)
			caret_ticker.restart();
		else if (caret_ticker.getElapsedTime().asMilliseconds() < 300)
		{
			string caret = "  ";
			for (int a = 0; a < caret_position; a++)
				caret += " ";
			caret += "_";
			line.setString(caret);
			line.setPosition(4.0f, console_height - line_height);
			window->draw(line);
		}
	}
}


/*******************************************************************
 * CONSOLE::COMMAND CLASS FUNCTIONS
 *******************************************************************/

/* Console::Command::ConsoleCommand
 * Console::Command class constructor
 *******************************************************************/
Console::Command::Command(string name, void(*commandFunc)(vector<string>), int min_args, bool show_in_list)
{
	// Init variables
	this->name = name;
	this->commandFunc = commandFunc;
	this->min_args = min_args;
	this->show_in_list = show_in_list;

	// Add this command to the console
	addCommand(*this);
}

/* Console::Command::execute
 * Executes the console command
 *******************************************************************/
void Console::Command::execute(vector<string> args)
{
	// Only execute if we have the minimum args specified
	if (args.size() >= min_args)
		commandFunc(args);
	else
		logMessage(S_FMT("Missing command arguments, type \"cmdhelp %s\" for more information", CHR(name)));
}


/*******************************************************************
 * CONSOLE COMMANDS
 *******************************************************************/

/* Console Command - "echo"
 * A simple command to print the given arguments to the console.
 *******************************************************************/
CONSOLE_COMMAND (echo, 1, true)
{
	string echo = args[0];
	for (unsigned a = 1; a < args.size(); a++)
	{
		echo += " ";
		echo += args[a];
	}

	Console::logMessage(echo);
}

/* Console Command - "cmdlist"
 * Lists all valid console commands
 *******************************************************************/
CONSOLE_COMMAND (cmdlist, 0, true)
{
	Console::logMessage(S_FMT("%d Valid Commands:", Console::numCommands()));

	for (Console::Command& cmd : Console::commands)
	{
		if (cmd.showInList() || Global::debug)
			Console::logMessage(S_FMT("\"%s\" (%d args)", CHR(cmd.getName()), cmd.minArgs()));
	}
}

/* Console Command - "cvarlist"
 * Lists all cvars
 *******************************************************************/
CONSOLE_COMMAND (cvarlist, 0, true)
{
	// Get sorted list of cvars
	vector<string> list;
	get_cvar_list(list);
	sort(list.begin(), list.end());

	Console::logMessage(S_FMT("%d CVars:", list.size()));

	// Write list to console
	for (string cvar : list)
		Console::logMessage(cvar);
}
