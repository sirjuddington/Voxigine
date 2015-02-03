
/*******************************************************************
 * Voxigine - A simple voxel engine
 * Copyright(C) 2014 Simon Judd
 *
 * Email:       sirjuddington@gmail.com
 * Web:         https://github.com/sirjuddington/Voxigine
 * Filename:    Shader.cpp
 * Description: Simple wrapper classes for OpenGL shaders and programs
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
#include "glew/glew.h"
#include "Shader.h"
#include <iostream>
#include <fstream>


/*******************************************************************
 * SHADER CLASS FUNCTIONS
 *******************************************************************/

/* Shader::Shader
 * Shader class constructor
 *******************************************************************/
Shader::Shader(GLenum type)
{
	// Init variables
	this->type = type;
	valid = false;
}

/* Shader::~Shader
 * Shader class destructor
 *******************************************************************/
Shader::~Shader()
{
	if (valid)
		glDeleteShader(id);
}

/* Shader::openFile
 * Loads the shader from a text file
 *******************************************************************/
bool Shader::openFile(string filename)
{
	// Read file to string
	string shader_text;
	std::ifstream file(filename);
	if (file.is_open())
	{
		string line;
		while (std::getline(file, line))
			shader_text += line + '\n';
		file.close();
	}
	else
	{
		logMessage(1, "Error: Unable to open shader file \"%s\"", CHR(filename));
		return false;
	}

	// Create & compile shader
	id = glCreateShader(type);
	const char* shader_c_str = shader_text.c_str();
	glShaderSource(id, 1, &shader_c_str, nullptr);
	glCompileShader(id);

	// Check for compile errors
	GLint status;
	glGetShaderiv(id, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		// Log error info
		GLint info_length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_length);

		char* info = new char[info_length + 1];
		glGetShaderInfoLog(id, info_length, nullptr, info);

		logMessage(1, "Error: Shader\"%s\" compile failed. Details below:", CHR(filename));
		logMessage(1, info);

		delete[] info;

		return false;
	}

	// Compiled ok, set valid
	logMessage(1, "Shader \"%s\" compiled successfully", CHR(filename));
	valid = true;

	return true;
}


/*******************************************************************
 * SHADERPROGRAM CLASS FUNCTIONS
 *******************************************************************/

/* ShaderProgram::addShader
 * Adds a (unique) shader to the program
 *******************************************************************/
void ShaderProgram::addShader(Shader* shader)
{
	// Check shader is valid
	if (!shader || !shader->isValid())
		return;

	// Check shader isn't already in the program
	if (std::find(shaders.begin(), shaders.end(), shader) == shaders.end())
		shaders.push_back(shader);
}

/* ShaderProgram::link
 * Links the shader program
 *******************************************************************/
bool ShaderProgram::link()
{
	// Delete program if already linked
	if (valid)
	{
		glDeleteProgram(id);
		valid = false;
	}

	// Create and link program
	id = glCreateProgram();
	for (Shader* s : shaders)
		glAttachShader(id, s->getId());
	glLinkProgram(id);

	// Detach shaders
	for (Shader* s : shaders)
		glDetachShader(id, s->getId());

	// Check for link errors
	GLint status;
	glGetProgramiv(id, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		// Log error info
		GLint info_length;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &info_length);

		char* info = new char[info_length + 1];
		glGetProgramInfoLog(id, info_length, nullptr, info);

		logMessage(1, "Error: Shader Program link failed. Details below:");
		logMessage(1, info);

		delete[] info;

		return false;
	}

	// Linked ok, set valid
	logMessage(1, "Shader Program compiled successfully (%d shaders)", shaders.size());
	valid = true;

	//// Update uniforms list
	//uniforms.clear();
	//GLint num_uniforms = 0;
	//GLint uniform_max_length = 0;
	//glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &num_uniforms);
	//glGetProgramiv(id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniform_max_length);
	//vector<GLchar> name_raw(256);
	//for (int a = 0; a < num_uniforms; a++)
	//{
	//	GLint size = 0;
	//	GLenum type = 0;
	//	GLsizei length = 0;
	//	glGetActiveUniform(id, a, name_raw.size(), &length, &size, &type, &name_raw[0]);
	//	string name((char*)&name_raw[0], length - 1);

	//	uniforms.push_back(name);
	//}

	return true;
}
