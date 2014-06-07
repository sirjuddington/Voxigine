
#ifndef __MAIN_H__
#define __MAIN_H__

// System includes
#ifdef _MSC_VER
#include <windows.h>
#define _CRT_SECURE_NO_WARNINGS 1
typedef __int8 int8_t;
typedef unsigned __int8 uint8_t;
typedef __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
#include <stdint.h>
#endif
#include <SFML/System.hpp>

// Use std::string
#include <string>
using namespace std;

// Use std::vector
#include <vector>
using namespace std;

// Useful functions
string formatString(const string fmt, ...);
void logMessage(unsigned level, const string message, ...);
int stringCompareNoCase(string left, string right);
bool stringEqualNoCase(string left, string right);

// Shorthand defines
#define S_FMT formatString
#define S_CMPNOCASE stringEqualNoCase
#define CHR(s) (static_cast<const char*>((s).c_str()))

// Global includes
#include "CVar.h"

// Namespace to hold 'global' variables
namespace Global
{
	extern int log_verbosity;
	extern bool debug;
};

#endif//__MAIN_H__
