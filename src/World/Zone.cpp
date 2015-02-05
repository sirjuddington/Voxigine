
#include "Main.h"
#include "Zone.h"
#include "Cell.h"
#include "Utilities/Random.h"
#include "Utilities/Math.h"
#include "External/libnoise/noise.h"

Zone::Zone(unsigned width, unsigned height)
{
	_width = width;
	_height = height;
}

Zone::~Zone()
{
}

Cell* Zone::getCell(unsigned x, unsigned y)
{
	// Get cell at coordinate
	Cell* c = _cells[x * _width + y];

	// Create if needed
	if (c == nullptr)
	{
		c = new Cell(x, y);
		_cells[x * _width + y] = c;
	}

	return c;
}

float Zone::heightAt(unsigned x, unsigned y)
{
	Cell* cell = getCell(x / 32, y / 32);
	return cell->heightAt(0, x % 32, y % 32);
}

void Zone::setHeightAt(unsigned x, unsigned y, uint8_t height)
{
	Cell* cell = getCell(x / 32, y / 32);
	cell->setHeightAt(x % 32, y % 32, height);
}

void Zone::fillWithRandomNoise()
{
	for (unsigned x = 0; x < _width; x++)
	{
		for (unsigned y = 0; y < _height; y++)
		{
			Cell* c = getCell(x, y);
			c->generateRandom(0, 4);
			c->generateLod();
		}
	}
}

void Zone::generateTestLandscape()
{
	noise::module::RidgedMulti generator_mountains;
	generator_mountains.SetSeed(Random::generateInt(-5000, 5000));
	generator_mountains.SetFrequency(0.5);
	generator_mountains.SetNoiseQuality(noise::NoiseQuality::QUALITY_FAST);

	noise::module::Billow generator_land;
	generator_land.SetSeed(Random::generateInt(-5000, 5000));
	generator_land.SetFrequency(0.5);
	generator_land.SetPersistence(0.4);

	double noise_scale = 0.001;
	double max_height = 240.0;
	for (unsigned x = 0; x < _width * 32; x++)
	{
		for (unsigned y = 0; y < _height * 32; y++)
		{
			double mult1 = (generator_mountains.GetValue(x*noise_scale, y*noise_scale, 0.5)) - 0.3;
			mult1 = Math::clamp(mult1, 0, 1.2);
			double mult2 = 0.5 + (generator_land.GetValue(x*noise_scale*4, y*noise_scale*4, 0.5) * 0.5);
			mult2 = Math::clamp(mult2, 0, 1.2);

			uint8_t hm = uint8_t(255.0 * mult1);
			uint8_t hl = uint8_t(50 * mult2);
			setHeightAt(x, y, max(hm, hl));
		}
	}

	//uint32_t n_height_points = Random::generateUnsigned(30, 80);

	//vector<fpoint3_t> height_points;
	//for (unsigned a = 0; a < n_height_points; a++)
	//	height_points.push_back(fpoint3_t(Random::generateUnsigned(0, _width), Random::generateUnsigned(0, _height), Random::generateUnsigned(0, 20)));

	//for (unsigned x = 0; x < _width; x++)
	//{
	//	for (unsigned y = 0; y < _height; y++)
	//	{
	//		
	//	}
	//}

	//// Generate number of things to generate
	//uint32_t n = Random::generateUnsigned(30, 80);

	//for (unsigned a = 0; a < n; a++)
	//{
	//	// Pick a random spot to put a 'mountain'
	//	uint32_t mx = Random::generateUnsigned(0, _width * 32);
	//	uint32_t my = Random::generateUnsigned(0, _height * 32);
	//	logMessage(0, "midpoint %d,%d", mx, my);

	//	// Determine random size
	//	uint32_t height = Random::generateUnsigned(0, 255);
	//	uint32_t radius = Random::generateUnsigned(32, 256);
	//	logMessage(0, "height %d", height);
	//	logMessage(0, "radius %d", radius);

	//	// Generate it
	//	unsigned x1 = max(0, (int)mx - (int)radius);
	//	unsigned x2 = min(mx + radius, _width * 32);
	//	unsigned y1 = max(0, (int)my - (int)radius);
	//	unsigned y2 = min(my + radius, _height * 32);
	//	logMessage(0, "loop %d,%d -> %d,%d", x1, y1, x2, y2);
	//	for (unsigned x = x1; x < x2; x++)
	//	{
	//		for (unsigned y = y1; y < y2; y++)
	//		{
	//			double dist = Math::distance(x, y, mx, my);
	//			double mult = (radius - dist) / radius;
	//			uint8_t h = (uint8_t)Math::clamp(height * mult, 0, height);
	//			if (heightAt(x, y) < h)
	//				setHeightAt(x, y, h);
	//		}
	//	}
	//}
}
