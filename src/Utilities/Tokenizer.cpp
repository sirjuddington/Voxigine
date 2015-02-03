
/*******************************************************************
 * Voxigine - A simple voxel engine
 * Copyright(C) 2014 Simon Judd
 *
 * Email:       sirjuddington@gmail.com
 * Web:         https://github.com/sirjuddington/Voxigine
 * Filename:    Tokenizer.cpp
 * Description: My trusty old string tokenizer class
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
#include "Tokenizer.h"


/*******************************************************************
 * TOKENIZER CLASS FUNCTIONS
 *******************************************************************/

/* Tokenizer::Tokenizer
 * Tokenizer class constructor
 *******************************************************************/
Tokenizer::Tokenizer(CommentTypes comments_style)
{
	// Initialize variables
	_current = nullptr;
	_start = nullptr;
	_size = 0;
	_comments = comments_style;
	_debug = false;
	_special = ";,:|={}/";	// Default special characters
	_name = "nothing";
	_line = 1;
	_token_start = 0;
	_token_end = 0;
	_decorate = false;
}

/* Tokenizer::~Tokenizer
 * Tokenizer class destructor
 *******************************************************************/
Tokenizer::~Tokenizer()
{
	// Free memory if used
	if (_start) free(_start);
}

/* Tokenizer::openFile
 * Reads a portion of a file to the Tokenizer
 *******************************************************************/
bool Tokenizer::openFile(string filename, uint32_t offset, uint32_t length)
{
	// Open the file
	FILE *fp = fopen(filename.c_str(), "rb");
	_name = filename;

	// Check file opened
	if (!fp)
	{
		logMessage(1, "Tokenizer::openFile: Unable to open file %s", CHR(filename));
		return false;
	}

	// If length isn't specified or exceeds the file length,
	// only read to the end of the file
	fseek(fp, 0, SEEK_END);
	if (offset + length > (unsigned)ftell(fp) || length == 0)
		length = ftell(fp) - offset;

	// Setup variables & allocate memory
	_size = length;
	_position = 0;
	_start = _current = (char*)malloc(_size);
	_end = _start + _size + 1;
	_line = 1;
	_token_start = 0;
	_token_end = 0;

	// Read the file portion
	fseek(fp, offset, SEEK_SET);
	fread(_start, 1, _size, fp);
	fclose(fp);

	return true;
}

/* Tokenizer::openString
 * Reads a portion of a string to the Tokenizer
 *******************************************************************/
bool Tokenizer::openString(string text, uint32_t offset, uint32_t length, string source)
{
	// If length isn't specified or exceeds the string's length,
	// only copy to the end of the string
	if (offset + length > (uint32_t) text.length() || length == 0)
		length = (uint32_t) text.length() - offset;

	// Setup variables & allocate memory
	_size = length;
	_position = 0;
	_line = 1;
	_token_start = 0;
	_token_end = 0;
	_start = _current = (char*)malloc(_size);
	_end = _start + _size + 1;
	_name = source;

	// Copy the string portion
	memcpy(_start, ((char*)text.c_str()) + offset, _size);

	return true;
}

/* Tokenizer::openMem
 * Reads a chunk of memory to the Tokenizer
 *******************************************************************/
bool Tokenizer::openMem(const char* mem, uint32_t length, string source)
{
	// Length must be specified
	if (length == 0)
	{
		logMessage(1, "Tokenizer::openMem: length not specified");
		return false;
	}

	_name = source;

	// Setup variables & allocate memory
	_size = length;
	_position = 0;
	_line = 1;
	_token_start = 0;
	_token_end = 0;
	_start = _current = (char*)malloc(_size);
	_end = _start + _size + 1;

	// Copy the data
	memcpy(_start, mem, _size);

	return true;
}

/* Tokenizer::openMem
 * Reads a chunk of memory to the Tokenizer
 *******************************************************************/
bool Tokenizer::openMem(const uint8_t* mem, uint32_t length, string source)
{
	// Length must be specified
	if (length == 0)
	{
		logMessage(1, "Tokenizer::openMem: length not specified");
		return false;
	}

	_name = source;

	// Setup variables & allocate memory
	_size = length;
	_position = 0;
	_line = 1;
	_token_start = 0;
	_token_end = 0;
	_start = _current = (char*)malloc(_size);
	_end = _start + _size + 1;

	// Copy the data
	memcpy(_start, mem, _size);

	return true;
}

/* Tokenizer::openMem
 * Reads a chunk of memory to the Tokenizer
 *******************************************************************/
//bool Tokenizer::openMem(MemChunk* mem, string source)
//{
//	// Needs to be valid
//	if (mem == nullptr)
//	{
//		wxLogMessage("Tokenizer::openMem: invalid MemChunk");
//		return false;
//	}
//
//	name = source;
//
//	// Setup variables & allocate memory
//	size = mem->getSize();
//	position = 0;
//	line = 1;
//	_token_start = 0;
//	_token_end = 0;
//	start = current = (char*) malloc(size);
//	end = start + size + 1;
//
//	// Copy the data
//	memcpy(start, mem->getData(), size);
//
//	return true;
//}

/* Tokenizer::isWhitespace
 * Checks if a character is 'whitespace'
 *******************************************************************/
bool Tokenizer::isWhitespace(char p)
{
	// Whitespace is either a newline, tab character or space
	if (p == '\n' || p == 13 || p == ' ' || p == '\t')
		return true;
	else
		return false;
}

/* Tokenizer::isSpecialCharacter
 * Checks if a character is a 'special' character, ie a character
 * that should always be its own token (;, =, | etc)
 *******************************************************************/
bool Tokenizer::isSpecialCharacter(char p)
{
	// Check through special tokens string
	for (unsigned a = 0; a < _special.size(); a++)
	{
		if (_special[a] == p)
			return true;
	}

	return false;
}

/* Tokenizer::incrementCurrent
 * Increments the position pointer, returns false on end of block
 *******************************************************************/
bool Tokenizer::incrementCurrent()
{
	if (_position >= _size - 1)
	{
		// At end of text, return false
		_position = _size;
		return false;
	}
	else
	{
		// Check for newline
		if (_current[0] == '\n')
			_line++;

		// Increment position & current pointer
		_position++;
		_current++;
		_token_end++;
		return true;
	}
}

/* Tokenizer::skipLineComment
 * Skips a '//' comment
 *******************************************************************/
void Tokenizer::skipLineComment()
{
	// Increment position until a newline or end is found
	while (_current[0] != '\n' && _current[0] != 13)
	{
		if (!incrementCurrent())
			return;
	}

	// Skip the newline character also
	incrementCurrent();
}

/* Tokenizer::skipMultilineComment
 * Skips a multiline comment (like this one :P)
 *******************************************************************/
void Tokenizer::skipMultilineComment()
{
	// Increment position until '*/' or end is found
	while (!(_current[0] == '*' && _current[1] == '/'))
	{
		if (!incrementCurrent())
			return;
	}

	// Skip the ending '*/'
	incrementCurrent();
	incrementCurrent();
}

/* Tokenizer::readToken
 * Reads the next 'token' from the text & moves past it
 *******************************************************************/
void Tokenizer::readToken(bool toeol)
{
	_token_current.clear();
	bool ready = false;
	_qstring = false;

	// Increment pointer to next token
	while (!ready)
	{
		ready = true;

		// Increment pointer until non-whitespace is found
		while (isWhitespace(_current[0]))
		{
			// Return if end of text found
			if (!incrementCurrent())
				return;
		}

		// Skip C-style comments
		if (_comments & CCOMMENTS)
		{
			// Check if we have a line comment
			if (_current + 1 < _end && _current[0] == '/' && _current[1] == '/')
			{
				ready = false;

				// DECORATE //$ handling
				if (!_decorate)
					skipLineComment();
				else if (_current + 2 < _end && _current[2] != '$')
					skipLineComment();
				else
					ready = true;
			}

			// Check if we have a multiline comment
			if (_current + 1 != _end && _current[0] == '/' && _current[1] == '*')
			{
				skipMultilineComment(); // Skip it
				ready = false;
			}
		}

		// Skip '##' comments
		if (_comments & DCOMMENTS)
		{
			if (_current + 1 != _end && _current[0] == '#' && _current[1] == '#')
			{
				skipLineComment(); // Skip it
				ready = false;
			}
		}

		// Skip '#' comments
		if (_comments & HCOMMENTS)
		{
			if (_current + 1 != _end && _current[0] == '#')
			{
				skipLineComment(); // Skip it
				ready = false;
			}
		}

		// Skip ';' comments
		if (_comments & SCOMMENTS)
		{
			if (_current[0] == ';')
			{
				skipLineComment(); // Skip it
				ready = false;
			}
		}

		// Check for end of text
		if (_position == _size)
			return;
	}

	// Init token delimiters
	_token_start = _position;
	_token_end = _position;

	// If we're at a special character, it's our token
	if (isSpecialCharacter(_current[0]))
	{
		_token_current += _current[0];
		_token_end = _position + 1;
		incrementCurrent();
		return;
	}

	// Now read the token
	if (_current[0] == '\"')   // If we have a literal string (enclosed with "")
	{
		_qstring = true;

		// Skip opening "
		incrementCurrent();

		// Read literal string (include whitespace)
		while (_current[0] != '\"')
		{
			_token_current += _current[0];

			if (!incrementCurrent())
				return;
		}

		// Skip closing "
		incrementCurrent();
	}
	else
	{
		// Read token (don't include whitespace)
		while (!((!toeol && isWhitespace(_current[0])) || _current[0] == '\n'))
		{
			// Return if special character found
			if (!toeol && isSpecialCharacter(_current[0]))
				return;

			// Add current character to the token
			_token_current += _current[0];

			// Return if end of text found
			if (!incrementCurrent())
				return;
		}
	}

	// Write token to log if debug mode enabled
	if (_debug)
		logMessage(3, _token_current);

	// Return the token
	return;
}

/* Tokenizer::skipToken
 * Reads the next 'token' from the text & moves past it
 *******************************************************************/
void Tokenizer::skipToken()
{
	readToken();
}

/* Tokenizer::getToken
 * Gets the next 'token' from the text & moves past it, returns
 * a blank string if we're at the end of the text
 *******************************************************************/
string Tokenizer::getToken()
{
	readToken();
	return _token_current;
}

/* Tokenizer::getLine
 * Gets the rest of the line (including whitespace) as a single token
 * and moves past it, returns a blank string if we're at the end of 
 * the text
 *******************************************************************/
string Tokenizer::getLine()
{
	readToken(true);
	return _token_current;
}

/* Tokenizer::getToken
 * Gets the next 'token' from the text & moves past it, writes
 * the result to [s]
 *******************************************************************/
void Tokenizer::getToken(string* s)
{
	// Read token
	readToken();

	// Set string value
	*s = _token_current;
}

/* Tokenizer::peekToken
 * Returns the next token without actually moving past it
 *******************************************************************/
string Tokenizer::peekToken()
{
	// Backup current position
	char* c = _current;
	uint32_t p = _position;
	int oline = _line;

	// Read the next token
	readToken();

	// Go back to original position
	_current = c;
	_position = p;
	_line = oline;

	// Return the token
	return _token_current;
}

/* Tokenizer::checkToken
 * Compares the current token with a string
 *******************************************************************/
bool Tokenizer::checkToken(string check)
{
	readToken();
	return !(_token_current.compare(check));
}

/* Tokenizer::getInteger
 * Reads a token and returns its integer value
 *******************************************************************/
int Tokenizer::getInteger()
{
	// Read token
	readToken();

	// Return integer value
	return atoi(CHR(_token_current));
}

/* Tokenizer::getInteger
 * Reads a token and writes its integer value to [i]
 *******************************************************************/
void Tokenizer::getInteger(int* i)
{
	// Read token
	readToken();

	// Set integer value
	*i = atoi(CHR(_token_current));
}

/* Tokenizer::getFloat
 * Reads a token and returns its floating point value
 *******************************************************************/
float Tokenizer::getFloat()
{
	// Read token
	readToken();

	// Return float value
	return (float)atof(CHR(_token_current));
}

/* Tokenizer::getFloat
 * Reads a token and writes its float value to [f]
 *******************************************************************/
void Tokenizer::getFloat(float* f)
{
	// Read token
	readToken();

	// Set float value
	*f = (float)atof(CHR(_token_current));
}

/* Tokenizer::getDouble
 * Reads a token and returns its double-precision floating point
 * value
 *******************************************************************/
double Tokenizer::getDouble()
{
	// Read token
	readToken();

	// Return double value
	return atof(CHR(_token_current));
}

/* Tokenizer::getDouble
 * Reads a token and writes its double value to [d]
 *******************************************************************/
void Tokenizer::getDouble(double* d)
{
	// Read token
	readToken();

	// Set double value
	*d = atof(CHR(_token_current));
}

/* Tokenizer::getBool
 * Reads a token and returns its boolean value, anything except
 * "0", "no", or "false" will return true
 *******************************************************************/
bool Tokenizer::getBool()
{
	// Read token
	readToken();

	// If the token is a string "no" or "false", the value is false
	if (S_CMPNOCASE(_token_current, "no") || S_CMPNOCASE(_token_current, "false"))
		return false;

	// Returns true ("1") or false ("0")
	return !!atoi(CHR(_token_current));
}

/* Tokenizer::getBool
 * Reads a token and writes its boolean value to [b], same rules as
 * getBool above
 *******************************************************************/
void Tokenizer::getBool(bool* b)
{
	// Read token
	readToken();

	// If the token is a string "no" or "false", the value is false
	if (S_CMPNOCASE(_token_current, "no") || S_CMPNOCASE(_token_current, "false"))
		*b = false;
	else
		*b = !!atoi(CHR(_token_current));
}

void Tokenizer::skipSection(string open, string close)
{
	int level = 0;
	readToken();
	while (!(_token_current.empty() && !_qstring))
	{
		// Increase depth level if another opener
		if (_token_current == open)
			level++;

		// Check for section closer
		else if (_token_current == close)
		{
			if (level == 0)
				break;
			else
				level--;
		}

		readToken();
	}
}
