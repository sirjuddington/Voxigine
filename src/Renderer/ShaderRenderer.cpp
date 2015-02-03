
#include "Main.h"
#include "glew/glew.h"
#include "ShaderRenderer.h"
#include "Camera.h"
#include "World/Cell.h"

const float vertexPositions[] ={
	0.75f, 0.75f, 0.0f, 1.0f,
	0.75f, -0.75f, 0.0f, 1.0f,
	-0.75f, -0.75f, 0.0f, 1.0f,
};

const Renderer::glvertex_t vertices[] ={
	Renderer::glvertex_t(0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
	Renderer::glvertex_t(0.5f, -0.366f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),
	Renderer::glvertex_t(-0.5f, -0.366f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f),
};

ShaderRenderer::ShaderRenderer() : _shader_vertex(GL_VERTEX_SHADER), _shader_fragment(GL_FRAGMENT_SHADER)
{
}

ShaderRenderer::~ShaderRenderer()
{
}

bool ShaderRenderer::init()
{
	bool ok = true;

	glGenBuffers(1, &_position_buffer_object);

	glBindBuffer(GL_ARRAY_BUFFER, _position_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &_vao);

	_shader_vertex.openFile("shaders/vertex.glsl");
	_shader_fragment.openFile("shaders/fragment.glsl");
	
	_program.addShader(&_shader_vertex);
	_program.addShader(&_shader_fragment);
	if (!_program.link())
		ok = false;

	// Setup perspective
	if (ok)
	{
		GLint uniform_perspective = glGetUniformLocation(_program.getId(), "mat_perspective");
		glUseProgram(_program.getId());
		glUniformMatrix4fv(uniform_perspective, 1, GL_FALSE, perspectiveMatrix(1.0f, 0.1f, 100.0f).data());
		glUseProgram(0);
	}

	return ok;
}

void ShaderRenderer::renderScene(int width, int height)
{
	glUseProgram(_program.getId());
	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _position_buffer_object);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8*sizeof(float), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)16);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
	glBindVertexArray(0);
}

void ShaderRenderer::renderCell(Cell* cell)
{
}

mat4f_t	ShaderRenderer::perspectiveMatrix(float scale, float z_near, float z_far)
{
	mat4f_t ret;

	ret[0] = scale;
	ret[5] = scale;
	ret[10] = (z_far + z_near) / (z_near - z_far);
	ret[14] = (2 * z_far * z_near) / (z_near - z_far);
	ret[11] = -1.0f;

	return ret;
}
