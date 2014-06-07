
/*******************************************************************
 * Voxigine - A simple voxel engine
 * Copyright(C) 2014 Simon Judd
 *
 * Email:       sirjuddington@gmail.com
 * Web:         https://github.com/sirjuddington/Voxigine
 * Filename:    Main.cpp
 * Description: The main loop and stuff
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
#include "Engine.h"
#include <algorithm>


/*******************************************************************
 * VARIABLES
 *******************************************************************/
namespace Global
{
	int log_verbosity = 1;
#ifdef DEBUG
	bool debug = true;
#else
	bool debug = false;
#endif
}


/*******************************************************************
 * FUNCTIONS
 *******************************************************************/

/* main
 * The program entry point and main loop
 *******************************************************************/
int main()
{
	// Init Engine
	if (!Engine::init())
		return 1;

	// Main Loop
	while (Engine::mainLoop());

	// Shutdown
	Engine::shutDown();

	return 0;
}

/* formatString
 * Formats a string, using c-style % arguments
 *******************************************************************/
string formatString(const string fmt, ...)
{
	std::vector<char> str(100, '\0');
	va_list ap;

	while (1)
	{
		va_start(ap, fmt);
		auto n = vsnprintf(str.data(), str.size(), fmt.c_str(), ap);
		va_end(ap);

		if ((n > -1) && (size_t(n) < str.size()))
			return str.data();

		if (n > -1)
			str.resize(n + 1);
		else
			str.resize(str.size() * 2);
	}

	return str.data();
}

/* logMessage
 * Adds a log message, if [level] is at or below the currently set
 * log verbosity level
 *******************************************************************/
void logMessage(unsigned level, const string message, ...)
{
	// Format string
	std::vector<char> str(100, '\0');
	va_list ap;
	while (1)
	{
		va_start(ap, message);
		auto n = vsnprintf(str.data(), str.size(), message.c_str(), ap);
		va_end(ap);

		if ((n > -1) && (size_t(n) < str.size()))
			break;

		if (n > -1)
			str.resize(n + 1);
		else
			str.resize(str.size() * 2);
	}

	// Log
	sf::err() << str.data() << "\n";
	Console::logMessage(str.data());
}

int stringCompareNoCase(string left, string right)
{
	// Yay C++11
	transform(left.begin(), left.end(), left.begin(), toupper);
	transform(right.begin(), right.end(), right.begin(), toupper);
	return left.compare(right);
}

bool stringEqualNoCase(string left, string right)
{
	if (left.length() == right.length())
	{
		transform(left.begin(), left.end(), left.begin(), toupper);
		transform(right.begin(), right.end(), right.begin(), toupper);
		return left == right;
	}

	return false;
}
