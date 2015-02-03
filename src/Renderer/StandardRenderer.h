
#ifndef __STANDARD_RENDERER_H__
#define __STANDARD_RENDERER_H__

#include "Renderer.h"
#include "OpenGL.h"
#include <unordered_map>

class Cell;
class RenderCell
{
private:
	Cell*				_cell;
	unsigned			_vbo_vertices;
	unsigned			_vbo_indices;
	unsigned			_n_quads;
	OpenGL::vertex_t*	_vertices;

public:
	RenderCell(Cell* cell);
	~RenderCell();

	void		generateVBO();
	void		render();
};

class StandardRenderer : public Renderer
{
public:
	StandardRenderer();
	~StandardRenderer();

	bool	init();
	void	renderScene(int width, int height);
	void	renderCell(Cell* cell);
	void	renderBlock(float x, float y, float top, float bottom, rgba_t colour, float size = 1.0f);

private:
	std::unordered_map<uint64_t, RenderCell*> _render_cells;
};

#endif//__STANDARD_RENDERER_H__
