
#ifndef __SHADER_RENDERER_H__
#define __SHADER_RENDERER_H__

#include "Shader.h"
#include "Renderer.h"

class Camera;
class ShaderRenderer : public Renderer
{
private:
	GLuint	_position_buffer_object;
	GLuint	_vao;

	Shader			_shader_vertex;
	Shader			_shader_fragment;
	ShaderProgram	_program;

public:
	ShaderRenderer();
	~ShaderRenderer();

	bool	init();
	void	renderScene(int width, int height);
	void	renderCell(Cell* cell);
	mat4f_t	perspectiveMatrix(float scale, float z_near, float z_far);
};

#endif//__SHADER_RENDERER_H__
