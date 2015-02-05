
#ifndef __CELL_H__
#define __CELL_H__

class Cell
{
private:
	int		_zone_x;
	int		_zone_y;
	float	_base_height;
	uint8_t	_height[32][32];
	uint8_t	_height_lod1[16][16];
	uint8_t	_height_lod2[8][8];
	uint8_t	_height_lod3[4][4];
	bool	_lod_generated;

public:
	Cell(int zone_x, int zone_y);
	~Cell();

	int		zoneX() { return _zone_x; }
	int		zoneY() { return _zone_y; }
	float	heightAt(uint8_t lod, uint8_t x, uint8_t y);

	void	setHeightAt(uint8_t x, uint8_t y, uint8_t height);

	void	generateLod();
	uint8_t	average(unsigned x1, unsigned y1, unsigned x2, unsigned y2);
	void	generateRandom(uint8_t min, uint8_t max);

	// testing
	void	generateBump();
	
};

#endif//__CELL_H__
