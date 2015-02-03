
#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "Camera.h"

class Cell;
class Renderer
{
protected:
	Camera	_camera;

public:
	struct glvertex_t
	{
		float x, y, z, w;
		float r, g, b, a;
		glvertex_t(float data[8])
		{
			x = data[0]; y = data[1]; z = data[2]; w = data[3];
			r = data[4]; g = data[5]; b = data[6]; a = data[7];
		}
		glvertex_t(float x, float y, float z, float r, float g, float b, float a)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = 1.0f;
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}
	};

	Renderer() {}
	virtual ~Renderer() {}

	Camera&			getCamera() { return _camera; }

	virtual bool	init() = 0;
	virtual void	renderScene(int width, int height) = 0;
	virtual void	renderCell(Cell* cell) = 0;
};

#endif//__RENDERER_H__
