
#include "Main.h"
#include "glew/glew.h"
#include "StandardRenderer.h"
#include "Camera.h"
#include "Utilities/Math.h"
#include "World/Cell.h"
#include "World/Zone.h"

EXTERN_CVAR(Float, max_view_distance)

// testing
//Cell test_cell;
rgba_t col_sky(70, 130, 240);
Zone test_zone(128, 128);
#define TEST_DIM 64

RenderCell::RenderCell(Cell* cell)
{
	_cell = cell;
	_vbo_vertices = 0;
	_vbo_indices = 0;
	_n_quads = 0;
	_current_lod_level = 0;
}

RenderCell::~RenderCell()
{
}

void RenderCell::generateVBO(uint8_t lod_level)
{
	// Create VBOs if needed
	if (_vbo_vertices == 0)
		glGenBuffers(1, &_vbo_vertices);
	//if (_vbo_indices == 0)
	//	glGenBuffers(1, &_vbo_indices);

	unsigned dim = 32;
	float size = 1.0f;
	rgba_t col_top(40, 130, 40, 255);
	if (lod_level == 1)
	{
		dim = 16;
		size = 2.0f;
		col_top.set(130, 100, 40, 255);
	}
	else if (lod_level == 2)
	{
		dim = 8;
		size = 4.0f;
		col_top.set(130, 100, 180, 255);
	}
	else if (lod_level == 3)
	{
		dim = 4;
		size = 8.0f;
		col_top.set(40, 150, 100, 255);
	}

	_vertices = new OpenGL::vertex_t[dim*dim*20];
	_n_quads = 0;
	//vector<OpenGL::vertex_t> vertices;

	rgba_t col_v = col_top.ampf(0.8f, 0.8f, 0.8f, 1.0f);
	rgba_t col_h = col_top.ampf(0.7f, 0.7f, 0.7f, 1.0f);

	unsigned v = 0;
	for (unsigned x = 0; x < dim; x++)
	{
		float xf = _cell->zoneX() * 32.0f + ((float)x * size);
		for (unsigned y = 0; y < dim; y++)
		{
			float yf = _cell->zoneY() * 32.0f + ((float)y * size);
			float top = _cell->heightAt(lod_level, x, y);

			// Top
			_vertices[v++].set(xf, yf, top, col_top);
			_vertices[v++].set(xf + size, yf, top, col_top);
			_vertices[v++].set(xf + size, yf + size, top, col_top);
			_vertices[v++].set(xf, yf + size, top, col_top);
			_n_quads++;

			// Y-Axis
			if (x == 0)
			{
				_vertices[v++].set(xf, yf, top, col_v);
				_vertices[v++].set(xf, yf + size, top, col_v);
				_vertices[v++].set(xf, yf + size, 0.0f, col_v);
				_vertices[v++].set(xf, yf, 0.0f, col_v);
				_n_quads++;
			}
			else
			{
				float bottom = _cell->heightAt(lod_level, x - 1, y);
				_vertices[v++].set(xf, yf, top, col_v);
				_vertices[v++].set(xf, yf + size, top, col_v);
				_vertices[v++].set(xf, yf + size, bottom, col_v);
				_vertices[v++].set(xf, yf, bottom, col_v);
				_n_quads++;
			}

			if (x == dim - 1)
			{
				_vertices[v++].set(xf + size, yf, 0.0f, col_v);
				_vertices[v++].set(xf + size, yf + size, 0.0f, col_v);
				_vertices[v++].set(xf + size, yf + size, top, col_v);
				_vertices[v++].set(xf + size, yf, top, col_v);
				_n_quads++;
			}

			// X-Axis
			if (y == 0)
			{
				_vertices[v++].set(xf, yf, 0.0f, col_h);
				_vertices[v++].set(xf + size, yf, 0.0f, col_h);
				_vertices[v++].set(xf + size, yf, top, col_h);
				_vertices[v++].set(xf, yf, top, col_h);
				_n_quads++;
			}
			else
			{
				float bottom = _cell->heightAt(lod_level, x, y - 1);
				_vertices[v++].set(xf, yf, bottom, col_h);
				_vertices[v++].set(xf + size, yf, bottom, col_h);
				_vertices[v++].set(xf + size, yf, top, col_h);
				_vertices[v++].set(xf, yf, top, col_h);
				_n_quads++;
			}

			if (y == dim - 1)
			{
				_vertices[v++].set(xf, yf + size, top, col_h);
				_vertices[v++].set(xf + size, yf + size, top, col_h);
				_vertices[v++].set(xf + size, yf + size, 0.0f, col_h);
				_vertices[v++].set(xf, yf + size, 0.0f, col_h);
				_n_quads++;
			}
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, _vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, _n_quads*4*sizeof(OpenGL::vertex_t), _vertices, GL_STATIC_DRAW);

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 32*32*4*sizeof(uint16_t), indices, GL_STATIC_DRAW);*/

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] _vertices;
	_vertices = nullptr;
	//delete[] indices;

	//logMessage(0, "VBO has %d quads - %d bytes", _n_quads, _n_quads*4*sizeof(OpenGL::vertex_t));
}

void RenderCell::unloadVBO()
{
	if (_vbo_vertices != 0)
		glDeleteBuffers(1, &_vbo_vertices);
	if (_vbo_indices != 0)
		glDeleteBuffers(1, &_vbo_indices);
	_vbo_vertices = 0;
	_vbo_indices = 0;
}

void RenderCell::render(fpoint3_t cam_position)
{
	// Determine LOD to use
	uint8_t lod_level = 3;
	double distance = Math::distance(cam_position.x, cam_position.y, 16.0 + _cell->zoneX() * 32.0, 16.0 + _cell->zoneY() * 32.0);
	if (distance < max_view_distance * 0.2)
		lod_level = 0;
	else if (distance < max_view_distance * 0.4)
		lod_level = 1;
	else if (distance < max_view_distance * 0.7)
		lod_level = 2;
	else if (distance < max_view_distance)
		lod_level = 3;
	else
	{
		unloadVBO();
		return;
	}

	if (_vbo_vertices == 0 || _current_lod_level != lod_level)
	{
		generateVBO(lod_level);
		_current_lod_level = lod_level;
	}

	glBindBuffer(GL_ARRAY_BUFFER, _vbo_vertices);
	glVertexPointer(3, GL_FLOAT, 24, 0);
	glColorPointer(3, GL_FLOAT, 24, ((char*)nullptr + 12));

	glDrawArrays(GL_QUADS, 0, _n_quads*4);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo_indices);
	//glIndexPointer(GL_UNSIGNED_SHORT, 0, 0);

	//glDrawElements(GL_QUADS, 32*32*4, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


StandardRenderer::StandardRenderer()
{
}

StandardRenderer::~StandardRenderer()
{
}

bool StandardRenderer::init()
{
	_camera.set(fpoint3_t(0.0f, 0.0f, 2.0f), fpoint2_t(0.5f, 0.5f));
	//test_cell.generateRandom(0, 4);
	//for (unsigned x = 0; x < TEST_DIM; x++)
	//{
	//	for (unsigned y = 0; y < TEST_DIM; y++)
	//	{
	//		Cell* c = new Cell(x, y);
	//		c->generateRandom(0, 4);
	//		//c->generateBump();
	//		c->generateLod();
	//		test_cells.push_back(c);
	//	}
	//}
	//test_zone.fillWithRandomNoise();
	test_zone.generateTestLandscape();

	// Setup lighting
	float light_pos[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float light_ambient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float light_diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float light_specular[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float light_shininess[1] = { 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, light_shininess);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, light_specular);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, light_ambient);
	glShadeModel(GL_SMOOTH);

	return true;
}

void StandardRenderer::renderScene(int width, int height)
{
	// Calculate aspect ratio
	float aspect = ((float)width / (float)height);
	float fovy = 2 * (float)Math::radToDeg(atan(tan(Math::degToRad(90) / 2) / aspect));

	// Setup projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(fovy, aspect, 0.5, max_view_distance * 1.3);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Setup GL stuff
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDepthMask(GL_TRUE);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);

	// Setup fog
	GLfloat fogColor[4]={ col_sky.fr(), col_sky.fg(), col_sky.fb(), 1.0f };
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, fogColor);
	//glFogf(GL_FOG_DENSITY, 0.1f);
	glFogf(GL_FOG_START, max_view_distance * 0.4f);
	glFogf(GL_FOG_END, max_view_distance * 0.8f);
	//if (render_fog_quality)
		glHint(GL_FOG_HINT, GL_NICEST);
	//else
	//	glHint(GL_FOG_HINT, GL_FASTEST);
	glEnable(GL_FOG);

	// Apply camera view
	_camera.applyView();

	// Clear
	glClearColor(col_sky.fr(), col_sky.fg(), col_sky.fb(), 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Test cell
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glEnable(GL_COLOR_MATERIAL);

	//renderCell(&test_cell);

	frect_t cam_strafe;
	cam_strafe.tl = fpoint2_t(_camera.getPosition().x, _camera.getPosition().y) + fpoint2_t(_camera.getStrafe().x, _camera.getStrafe().y);
	cam_strafe.br = fpoint2_t(_camera.getPosition().x, _camera.getPosition().y) - fpoint2_t(_camera.getStrafe().x, _camera.getStrafe().y);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	for (unsigned x = 0; x < test_zone.getWidth(); x++)
	{
		//glPushMatrix();
		//glTranslatef((float)x * 32.0f, 0.0f, 0.0f);
		for (unsigned y = 0; y < test_zone.getHeight(); y++)
		{
			//renderCell(test_cells[x * TEST_DIM + y]);
			uint64_t index = x * test_zone.getHeight() + y;

			RenderCell* rc = _render_cells[index];
			if (!rc)
			{
				rc = new RenderCell(test_zone.getCell(x, y));
				_render_cells[index] = rc;
			}

			//glTranslatef(0.0f, 32.0f, 0.0f);

			rc->render(_camera.getPosition());
		}
		//glPopMatrix();
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_COLOR_MATERIAL);
}

void StandardRenderer::renderCell(Cell* cell)
{
	double distance = Math::distance(_camera.getPosition().x, _camera.getPosition().y, 16.0 + cell->zoneX() * 32.0, 16.0 + cell->zoneY() * 32.0);

	rgba_t colour(40, 130, 40, 255);

	// LOD 0 (none)
	if (distance < 128.0)
	{
		//glColor4f(0.2f, 0.5f, 0.2f, 1.0f);
		for (unsigned x = 0; x < 32; x++)
			for (unsigned y = 0; y < 32; y++)
				renderBlock((float)x, (float)y, cell->heightAt(0, x, y), 0.0f, colour, 1.0f);
	}
	
	// LOD 1
	else if (distance >= 128.0 && distance < 256.0)
	{
		//glColor4f(0.2f, 0.5f, 0.2f, 1.0f);
		for (unsigned x = 0; x < 16; x++)
			for (unsigned y = 0; y < 16; y++)
				renderBlock((float)x * 2.0f, (float)y * 2.0f, cell->heightAt(1, x, y), 0.0f, colour, 2.0f);
	}

	// LOD 2
	else if (distance >= 256.0 && distance < 512.0)
	{
		//glColor4f(0.2f, 0.5f, 0.2f, 1.0f);
		for (unsigned x = 0; x < 8; x++)
			for (unsigned y = 0; y < 8; y++)
				renderBlock((float)x * 4.0f, (float)y * 4.0f, cell->heightAt(2, x, y), 0.0f, colour, 4.0f);
	}

	// LOD 3
	else if (distance >= 512.0 && distance < 2048.0)
	{
		//glColor4f(0.2f, 0.5f, 0.2f, 1.0f);
		for (unsigned x = 0; x < 4; x++)
			for (unsigned y = 0; y < 4; y++)
				renderBlock((float)x * 8.0f, (float)y * 8.0f, cell->heightAt(3, x, y), 0.0f, colour, 8.0f);
	}
}

void StandardRenderer::renderBlock(float x, float y, float top, float bottom, rgba_t colour, float size)
{
	glBegin(GL_QUADS);

	glColor4f(colour.fr(), colour.fg(), colour.fb(), colour.fa());
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(x + size, y, top);
	glVertex3f(x + size, y + size, top);
	glVertex3f(x, y + size, top);
	glVertex3f(x, y, top);

	glColor4f(colour.fr() * 0.7f, colour.fg() * 0.7f, colour.fb() * 0.7f, colour.fa());
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(x, y, top);
	glVertex3f(x, y, bottom);
	glVertex3f(x + size, y, bottom);
	glVertex3f(x + size, y, top);

	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(x + size, y + size, top);
	glVertex3f(x + size, y + size, bottom);
	glVertex3f(x, y + size, bottom);
	glVertex3f(x, y + size, top);

	glColor4f(colour.fr() * 0.8f, colour.fg() * 0.8f, colour.fb() * 0.8f, colour.fa());
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(x + size, y, top);
	glVertex3f(x + size, y, bottom);
	glVertex3f(x + size, y + size, bottom);
	glVertex3f(x + size, y + size, top);

	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(x, y + size, top);
	glVertex3f(x, y + size, bottom);
	glVertex3f(x, y, bottom);
	glVertex3f(x, y, top);

	glEnd();
}
