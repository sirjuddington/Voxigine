
#include "Main.h"
#include "Cell.h"
#include "Utilities/Math.h"
#include "Utilities/Random.h"


Cell::Cell(int zone_x, int zone_y)
{
	_zone_x = zone_x;
	_zone_y = zone_y;
	_base_height = 0.0f;
	memset(_height, 0, 32 * 32);
	memset(_height_lod1, 0, 16 * 16);
	memset(_height_lod2, 0, 8 * 8);
	memset(_height_lod3, 0, 4 * 4);
	_lod_generated = false;
}

Cell::~Cell()
{
}

float Cell::heightAt(uint8_t lod, uint8_t x, uint8_t y)
{
	if (lod > 0 && !_lod_generated)
		generateLod();

	if (lod == 0)
		return _base_height + (float)_height[x][y];
	else if (lod == 1)
		return _base_height + (float)_height_lod1[x][y];
	else if (lod == 2)
		return _base_height + (float)_height_lod2[x][y];
	else if (lod == 3)
		return _base_height + (float)_height_lod3[x][y];

	return _base_height;
}

void Cell::setHeightAt(uint8_t x, uint8_t y, uint8_t height)
{
	_height[x][y] = height;
}

void Cell::generateLod()
{
	memset(_height_lod1, 0, 16 * 16);
	memset(_height_lod2, 0, 8 * 8);
	memset(_height_lod3, 0, 4 * 4);

	double lod1[16][16];
	double lod2[8][8];
	double lod3[4][4];
	memset(lod1, 0, 16*16*sizeof(double));
	memset(lod2, 0, 8*8*sizeof(double));
	memset(lod3, 0, 4*4*sizeof(double));

	unsigned x_l1 = 0;
	unsigned x_l2 = 0;
	unsigned x_l3 = 0;
	for (unsigned x = 0; x < 32; x++)
	{
		if (x > 0)
		{
			if (x % 2 == 0)
				x_l1++;
			if (x % 4 == 0)
				x_l2++;
			if (x % 8 == 0)
				x_l3++;
		}

		unsigned y_l1 = 0;
		unsigned y_l2 = 0;
		unsigned y_l3 = 0;
		for (unsigned y = 0; y < 32; y++)
		{
			if (y > 0)
			{
				if (y % 2 == 0)
					y_l1++;
				if (y % 4 == 0)
					y_l2++;
				if (y % 8 == 0)
					y_l3++;
			}

			lod1[x_l1][y_l1] += _height[x][y];
			lod2[x_l2][y_l2] += _height[x][y];
			lod3[x_l3][y_l3] += _height[x][y];
		}
	}

	// LOD 1
	for (unsigned x = 0; x < 16; x++)
		for (unsigned y = 0; y < 16; y++)
			_height_lod1[x][y] = (uint8_t)Math::clamp(lod1[x][y] / 4.0, 0.0, 255.0);

	// LOD 2
	for (unsigned x = 0; x < 8; x++)
		for (unsigned y = 0; y < 8; y++)
			_height_lod2[x][y] = (uint8_t)Math::clamp(lod2[x][y] / 16.0, 0.0, 255.0);

	// LOD 3
	for (unsigned x = 0; x < 4; x++)
		for (unsigned y = 0; y < 4; y++)
			_height_lod3[x][y] = (uint8_t)Math::clamp(lod3[x][y] / 64.0, 0.0, 255.0);

	_lod_generated = true;
}

uint8_t Cell::average(unsigned x1, unsigned y1, unsigned x2, unsigned y2)
{
	unsigned avg = 0;
	unsigned num = 0;
	for (unsigned x = x1; x < x2; x++)
	{
		for (unsigned y = y1; y < y2; y++)
		{
			avg += _height[x][y];
			num++;
		}
	}

	return avg / num;
}

void Cell::generateRandom(uint8_t min, uint8_t max)
{
	vector<uint32_t> heights = Random::generateUnsignedArray(32 * 32, 0, 4);
	
	for (unsigned x = 0; x < 32; x++)
		for (unsigned y = 0; y < 32; y++)
			_height[x][y] = heights[x * 32 + y];
}

void Cell::generateBump()
{
	for (unsigned x = 0; x < 32; x++)
	{
		for (unsigned y = 0; y < 32; y++)
		{
			double dist = Math::distance(16, 16, x, y);
			_height[x][y] = Math::clamp(6 - (dist * 0.7 * 0.5), 0, 6) + 1;
		}
	}
}
