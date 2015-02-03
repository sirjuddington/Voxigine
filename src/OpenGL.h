
#ifndef __OPENGL_H__
#define __OPENGL_H__

namespace OpenGL
{
	struct vertex_t
	{
		float x, y, z;
		float r, g, b;

		vertex_t()
		{
			x = y = z = 0;
			r = g = b = 1.0f;
		}

		vertex_t(float x, float y, float z)
		{
			set(x, y, z);
		}

		vertex_t(float x, float y, float z, rgba_t colour)
		{
			set(x, y, z, colour);
		}

		void set(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		void set(float x, float y, float z, rgba_t colour)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->r = colour.fr();
			this->g = colour.fg();
			this->b = colour.fb();
		}
	};

	struct quad_vbo_t
	{
		unsigned	vertices_id;
		unsigned	indices_id;
		unsigned	n_quads;
	};
}

#endif//__OPENGL_H__
