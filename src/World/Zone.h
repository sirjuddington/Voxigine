
#ifndef __ZONE_H__
#define __ZONE_H__

#include <unordered_map>

class Cell;
typedef std::unordered_map<uint64_t, Cell*> CellMap;

class Zone
{
public:
	Zone(unsigned width, unsigned height);
	~Zone();

	unsigned	getWidth() { return _width; }
	unsigned	getHeight() { return _height; }
	Cell*		getCell(unsigned x, unsigned y);
	float		heightAt(unsigned x, unsigned y);

	void	setHeightAt(unsigned x, unsigned y, uint8_t height);

	// Testing
	void	fillWithRandomNoise();
	void	generateTestLandscape();

private:
	unsigned	_width;
	unsigned	_height;
	CellMap		_cells;
};

#endif//__ZONE_H__
