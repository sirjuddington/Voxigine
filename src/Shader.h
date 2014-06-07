
#ifndef __SHADER_H__
#define __SHADER_H__

#include <SFML/OpenGL.hpp>

class Shader
{
private:
	GLuint	id;
	GLenum	type;
	bool	valid;

public:
	Shader(GLenum type);
	~Shader();

	bool	isValid() { return valid; }
	GLuint	getId() { return id; }
	GLenum	getType() { return type; }

	bool	openFile(string filename);
};

class ShaderProgram
{
private:
	vector<Shader*>	shaders;
	GLuint			id;
	bool			valid;

public:
	ShaderProgram() { valid = false; }
	~ShaderProgram() {}

	unsigned	nShaders() { return shaders.size(); }
	Shader*		getShader(unsigned index) { if (index < shaders.size()) return shaders[index]; else return nullptr; }
	GLuint		getId() { return id; }
	bool		isValid() { return valid; }

	void	addShader(Shader* shader);
	bool	link();
};

#endif//__SHADER_H__
