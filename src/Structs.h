
#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <math.h>
using std::min;
using std::max;

// This is basically a collection of handy little structs I use a lot,
// with some useful functions for each of them

// point2_t: A 2d coordinate
struct point2_t
{
	int x, y;

	point2_t() { x = y = 0; }
	point2_t(int X, int Y) { x = X; y = Y; }

	void set(int X, int Y) { x = X; y = Y; }
	void set(point2_t p) { x = p.x; y = p.y; }

	point2_t operator+(point2_t point)
	{
		return point2_t(point.x + x, point.y + y);
	}

	point2_t operator-(point2_t point)
	{
		return point2_t(x - point.x, y - point.y);
	}

	point2_t operator*(float num)
	{
		return point2_t((int)((float)x * num), (int)((float)y * num));
	}

	point2_t operator/(float num)
	{
		if (num == 0)
			return point2_t(0, 0);
		else
			return point2_t((int)((float)x / num), (int)((float)y / num));
	}

	bool operator==(point2_t rhs)
	{
		return (x == rhs.x && y == rhs.y);
	}
};

// fpoint2_t: A 2d coordinate (or vector) with floating-point precision
struct fpoint2_t
{
	float x, y;

	fpoint2_t() { x = y = 0.0f; }
	fpoint2_t(float X, float Y) { x = X; y = Y; }
	fpoint2_t(point2_t p) { x = (float)p.x; y = (float)p.y; }

	void set(float X, float Y) { x = X; y = Y; }
	void set(fpoint2_t p) { x = p.x; y = p.y; }
	void set(point2_t p) { x = (float)p.x; y = (float)p.y; }

	float magnitude()
	{
		return sqrt((x * x) + (y * y));
	}

	fpoint2_t normalized()
	{
		float mag = magnitude();
		if (mag == 0.0f)
			return fpoint2_t(0.0f, 0.0f);
		else
			return fpoint2_t(x / mag, y / mag);
	}

	void normalize()
	{
		float mag = magnitude();
		if (mag == 0)
		{
			x = 0;
			y = 0;
		}
		else
		{
			x /= mag;
			y /= mag;
		}
	}

	float dot(fpoint2_t vec)
	{
		return x*vec.x + y*vec.y;
	}

	float cross(fpoint2_t p2)
	{
		return (x*p2.y) - (y*p2.x);
	}

	fpoint2_t operator+(fpoint2_t point)
	{
		return fpoint2_t(point.x + x, point.y + y);
	}

	fpoint2_t operator-(fpoint2_t point)
	{
		return fpoint2_t(x - point.x, y - point.y);
	}

	fpoint2_t operator*(float num)
	{
		return fpoint2_t(x * num, y * num);
	}

	fpoint2_t operator/(float num)
	{
		if (num == 0.0f)
			return fpoint2_t(0.0f, 0.0f);
		else
			return fpoint2_t(x / num, y / num);
	}

	bool operator==(fpoint2_t rhs)
	{
		return (x == rhs.x && y == rhs.y);
	}
};


// fpoint3_t: A 3d coordinate (or vector) with floating-point precision
struct fpoint3_t
{
	float x, y, z;

	fpoint3_t() { x = y = z = 0; }
	fpoint3_t(float X, float Y, float Z) { x = X; y = Y; z = Z; }
	fpoint3_t(fpoint2_t p) { x = p.x; y = p.y; z = 0; }

	void set(float X, float Y, float Z) { x = X; y = Y; z = Z; }
	void set(fpoint3_t p) { x = p.x; y = p.y; z = p.z; }

	float magnitude()
	{
		return sqrt((x * x) + (y * y) + (z * z));
	}

	float dot(fpoint3_t vec)
	{
		return x*vec.x + y*vec.y + z*vec.z;
	}

	fpoint3_t normalize()
	{
		float mag = magnitude();
		if (mag == 0)
			return fpoint3_t(0, 0, 0);
		else
			return fpoint3_t(x / mag, y / mag, z / mag);
	}

	float distance_to(fpoint3_t point)
	{
		float dist_x = point.x - x;
		float dist_y = point.y - y;
		float dist_z = point.z - z;

		return sqrt((dist_x * dist_x) + (dist_y * dist_y) + (dist_z * dist_z));
	}

	fpoint3_t operator+(fpoint3_t point)
	{
		return fpoint3_t(point.x + x, point.y + y, point.z + z);
	}

	fpoint3_t operator-(fpoint3_t point)
	{
		return fpoint3_t(x - point.x, y - point.y, z - point.z);
	}

	fpoint3_t operator*(float num)
	{
		return fpoint3_t(x * num, y * num, z * num);
	}

	fpoint3_t operator/(float num)
	{
		if (num == 0)
			return fpoint3_t(0, 0, 0);
		else
			return fpoint3_t(x / num, y / num, z / num);
	}

	fpoint3_t cross(fpoint3_t p2)
	{
		fpoint3_t cross_product;

		cross_product.x = ((y * p2.z) - (z * p2.y));
		cross_product.y = ((z * p2.x) - (x * p2.z));
		cross_product.z = ((x * p2.y) - (y * p2.x));

		return cross_product;
	}

	fpoint2_t get2d()
	{
		return fpoint2_t(x, y);
	}
};


// rgba_t: A 32-bit colour definition
struct rgba_t
{
	uint8_t r, g, b, a;
	char blend; // 0=normal, 1=additive

	// Constructors
	rgba_t() { r = 0; g = 0; b = 0; a = 0; blend = -1; }

	rgba_t(uint8_t R, uint8_t G, uint8_t B, uint8_t A = 255, char BLEND = -1)
	{
		r = R;
		g = G;
		b = B;
		a = A;
		blend = BLEND;
	}

	// Functions
	void set(uint8_t R, uint8_t G, uint8_t B, uint8_t A = 255, char BLEND = -1)
	{
		r = R;
		g = G;
		b = B;
		a = A;
		blend = BLEND;
	}

	void set(rgba_t colour)
	{
		r = colour.r;
		g = colour.g;
		b = colour.b;
		a = colour.a;
		blend = colour.blend;
	}

	float fr() { return (float)r / 255.0f; }
	float fg() { return (float)g / 255.0f; }
	float fb() { return (float)b / 255.0f; }
	float fa() { return (float)a / 255.0f; }

	bool equals(rgba_t rhs, bool alpha = false)
	{
		bool col_equal = (r == rhs.r && g == rhs.g && b == rhs.b);

		if (alpha)
			return col_equal && (a == rhs.a);
		else
			return col_equal;
	}

	// Amplify/fade colour components by given amounts
	rgba_t amp(int R, int G, int B, int A)
	{
		int nr = r + R;
		int ng = g + G;
		int nb = b + B;
		int na = a + A;

		if (nr > 255) nr = 255;
		if (nr < 0) nr = 0;
		if (ng > 255) ng = 255;
		if (ng < 0) ng = 0;
		if (nb > 255) nb = 255;
		if (nb < 0) nb = 0;
		if (na > 255) na = 255;
		if (na < 0) na = 0;

		return rgba_t((uint8_t)nr, (uint8_t)ng, (uint8_t)nb, (uint8_t)na, blend);
	}

	// Amplify/fade colour components by factors
	rgba_t ampf(float fr, float fg, float fb, float fa)
	{
		int nr = (int)(r * fr);
		int ng = (int)(g * fg);
		int nb = (int)(b * fb);
		int na = (int)(a * fa);

		if (nr > 255) nr = 255;
		if (nr < 0) nr = 0;
		if (ng > 255) ng = 255;
		if (ng < 0) ng = 0;
		if (nb > 255) nb = 255;
		if (nb < 0) nb = 0;
		if (na > 255) na = 255;
		if (na < 0) na = 0;

		return rgba_t((uint8_t)nr, (uint8_t)ng, (uint8_t)nb, (uint8_t)na, blend);
	}

	void write(uint8_t* ptr)
	{
		if (ptr)
		{
			ptr[0] = r;
			ptr[1] = g;
			ptr[2] = b;
			ptr[3] = a;
		}
	}

	// Returns a copy of this colour as greyscale (using 'common' component coefficients)
	rgba_t greyscale()
	{
		uint8_t l = uint8_t((float)r*0.3) + uint8_t((float)g*0.59) + uint8_t((float)b*0.11);
		return rgba_t(l, l, l, a, blend);
	}
};

// Some basic colours
#define COL_WHITE	rgba_t(255, 255, 255, 255, 0)
#define COL_BLACK	rgba_t(0, 0, 0, 255, 0)
#define COL_RED		rgba_t(255, 0, 0, 255, 0)
#define COL_GREEN	rgba_t(0, 255, 0, 255, 0)
#define COL_BLUE	rgba_t(0, 0, 255, 255, 0)
#define COL_YELLOW	rgba_t(255, 255, 0, 255, 0)
#define COL_PURPLE	rgba_t(255, 0, 255, 255, 0)
#define COL_CYAN	rgba_t(0, 255, 255, 255, 0)

// hsl_t: Represents a colour in HSL format, generally used for calculations
struct hsl_t
{
	float h, s, l;

	hsl_t() { h = s = l = 0; }
	hsl_t(float h, float s, float l)
	{
		this->h = h;
		this->s = s;
		this->l = l;
	}
};

// lab_t: Represents a colour in CIE-L*a*b format, generally used for calculations
struct lab_t
{
	float l, a, b;

	lab_t() { l = a = b = 0; }
	lab_t(float l, float a, float b)
	{
		this->l = l;
		this->a = a;
		this->b = b;
	}
};

// rect_t: A rectangle (int points)
struct rect_t
{
	point2_t tl, br;

	// Constructors
	rect_t() { tl.set(0, 0); br.set(0, 0); }

	rect_t(point2_t TL, point2_t BR)
	{
		tl.set(TL);
		br.set(BR);
	}

	rect_t(int x1, int y1, int x2, int y2)
	{
		tl.set(x1, y1);
		br.set(x2, y2);
	}

	rect_t(int x, int y, int width, int height, int align)
	{
		// Top-left
		if (align == 0)
		{
			tl.set(x, y);
			br.set(x + width, y + height);
		}

		// Centered
		else if (align == 1)
		{
			tl.set(x - (width / 2), y - (height / 2));
			br.set(x + (width / 2), y + (height / 2));
		}
	}

	// Functions
	void set(point2_t TL, point2_t BR)
	{
		tl.set(TL);
		br.set(BR);
	}

	void set(int x1, int y1, int x2, int y2)
	{
		tl.set(x1, y1);
		br.set(x2, y2);
	}

	void set(rect_t rect)
	{
		tl.set(rect.tl);
		br.set(rect.br);
	}

	int x1() { return tl.x; }
	int y1() { return tl.y; }
	int x2() { return br.x; }
	int y2() { return br.y; }

	int left()	{ return min(tl.x, br.x); }
	int top()		{ return min(tl.y, br.y); }
	int right()	{ return max(br.x, tl.x); }
	int bottom()	{ return max(br.y, tl.y); }

	int width() { return br.x - tl.x; }
	int height() { return br.y - tl.y; }

	int awidth() { return max(br.x, tl.x) - min(tl.x, br.x); }
	int aheight() { return max(br.y, tl.y) - min(tl.y, br.y); }

	point2_t middle() { return point2_t(left() + (awidth() / 2), top() + (aheight() / 2)); }

	void expand(int x, int y)
	{
		if (x1() < x2())
		{
			tl.x -= x;
			br.x += x;
		}
		else
		{
			tl.x += x;
			br.x -= x;
		}

		if (y1() < y2())
		{
			tl.y -= y;
			br.y += y;
		}
		else
		{
			tl.y += y;
			br.y -= y;
		}
	}

	float length()
	{
		float dist_x = float(x2() - x1());
		float dist_y = float(y2() - y1());

		return sqrt(dist_x * dist_x + dist_y * dist_y);
	}

	bool within(int x, int y)
	{
		if (x >= left() && x <= right() && y >= top() && y <= bottom())
			return true;
		else
			return false;
	}
};


// frect_t: A rectangle (float points)
struct frect_t
{
	fpoint2_t tl, br;

	// Constructors
	frect_t() { tl.set(0, 0); br.set(0, 0); }

	frect_t(rect_t rect)
	{
		tl.set(rect.tl);
		br.set(rect.br);
	}

	frect_t(fpoint2_t TL, fpoint2_t BR)
	{
		tl.set(TL);
		br.set(BR);
	}

	frect_t(point2_t TL, point2_t BR)
	{
		tl.set(TL);
		br.set(BR);
	}

	frect_t(float x1, float y1, float x2, float y2)
	{
		tl.set(x1, y1);
		br.set(x2, y2);
	}

	frect_t(float x, float y, float width, float height, int align)
	{
		// Top-left
		if (align == 0)
		{
			tl.set(x, y);
			br.set(x + width, y + height);
		}

		// Centered
		else if (align == 1)
		{
			tl.set(x - (width / 2), y - (height / 2));
			br.set(x + (width / 2), y + (height / 2));
		}
	}

	// Functions
	void set(rect_t rect)
	{
		tl.set(rect.tl);
		br.set(rect.br);
	}

	void set(fpoint2_t TL, fpoint2_t BR)
	{
		tl.set(TL);
		br.set(BR);
	}

	void set(point2_t TL, point2_t BR)
	{
		tl.set(TL);
		br.set(BR);
	}

	void set(float x1, float y1, float x2, float y2)
	{
		tl.set(x1, y1);
		br.set(x2, y2);
	}

	void set(frect_t rect)
	{
		tl.set(rect.tl);
		br.set(rect.br);
	}

	float x1() { return tl.x; }
	float y1() { return tl.y; }
	float x2() { return br.x; }
	float y2() { return br.y; }

	float left()	{ return min(tl.x, br.x); }
	float top()	{ return min(tl.y, br.y); }
	float right()	{ return max(br.x, tl.x); }
	float bottom()	{ return max(br.y, tl.y); }

	float width() { return br.x - tl.x; }
	float height() { return br.y - tl.y; }

	float awidth() { return max(br.x, tl.x) - min(tl.x, br.x); }
	float aheight() { return max(br.y, tl.y) - min(tl.y, br.y); }

	fpoint2_t middle() { return fpoint2_t(left() + (awidth() / 2), top() + (aheight() / 2)); }

	void expand(float x, float y)
	{
		if (x1() < x2())
		{
			tl.x -= x;
			br.x += x;
		}
		else
		{
			tl.x += x;
			br.x -= x;
		}

		if (y1() < y2())
		{
			tl.y -= y;
			br.y += y;
		}
		else
		{
			tl.y += y;
			br.y -= y;
		}
	}

	float length()
	{
		float dist_x = x2() - x1();
		float dist_y = y2() - y1();

		return sqrt(dist_x * dist_x + dist_y * dist_y);
	}

	bool within(float x, float y)
	{
		if (x >= left() && x <= right() && y >= top() && y <= bottom())
			return true;
		else
			return false;
	}
};


// plane_t: A 3d plane
struct plane_t
{
	float a, b, c, d;

	fpoint3_t normal()
	{
		fpoint3_t norm(a, b, c);
		return norm.normalize();
	}

	void normalize()
	{
		fpoint3_t vec(a, b, c);
		float mag = (float)vec.magnitude();
		a = a / mag;
		b = b / mag;
		c = c / mag;
		d = d / mag;
	}

	float height_at(float x, float y)
	{
		return ((-a * x) + (-b * y) + d) / c;
	}
};


// bbox_t: A simple bounding box with related functions
struct bbox_t
{
	fpoint2_t	min;
	fpoint2_t	max;

	bbox_t() { reset(); }

	void reset()
	{
		min.set(0, 0);
		max.set(0, 0);
	}

	void extend(float x, float y)
	{
		// Init bbox if it has been reset last
		if (min.x == 0 && min.y == 0 && max.x == 0 && max.y == 0)
		{
			min.set(x, y);
			max.set(x, y);
			return;
		}

		// Extend to fit the point [x,y]
		if (x < min.x)
			min.x = x;
		if (x > max.x)
			max.x = x;
		if (y < min.y)
			min.y = y;
		if (y > max.y)
			max.y = y;
	}

	bool point_within(float x, float y)
	{
		return (x >= min.x && x <= max.x && y >= min.y && y <= max.y);
	}

	bool is_within(fpoint2_t bmin, fpoint2_t bmax)
	{
		return (min.x >= bmin.x && max.x <= bmax.x && min.y >= bmin.y && max.y <= bmax.y);
	}

	bool is_valid()
	{
		return ((max.x - min.x > 0) && (max.y - min.y) > 0);
	}

	fpoint2_t size()
	{
		return fpoint2_t(max.x - min.x, max.y - min.y);
	}

	float width()
	{
		return max.x - min.x;
	}

	float height()
	{
		return max.y - min.y;
	}

	fpoint2_t mid()
	{
		return fpoint2_t(mid_x(), mid_y());
	}

	float mid_x()
	{
		return min.x + ((max.x - min.x) * 0.5f);
	}

	float mid_y()
	{
		return min.y + ((max.y - min.y) * 0.5f);
	}
};

// key_value_t: Simple key-value pair (string-string)
struct key_value_t
{
	string	key;
	string	value;

	key_value_t(string key, string value = "")
	{
		this->key = key;
		this->value = value;
	}
};

struct mat4f_t
{
	float	values[16];

	mat4f_t()
	{
		memset(values, 0, sizeof(float) * 16);
	}

	float* data() { return values; }

	float& operator[](int index)
	{
		if (index >= 0 && index < 16)
			return values[index];
		else
			return values[0];
	}
};

#endif//__STRUCTS_H__
