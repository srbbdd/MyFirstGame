#pragma once
#include "map.h"
#include "FOV.h"
class FOV
{
public:
	static constexpr int RADIUS = 11;
	static void compute(Map& map, int originXint,int originY);
private:
	static const int MULT[8][4];
	static int castLight(Map& map, int cx, int cy, int row, float startSlope, float endSlope, int xx, int xy, int yx, int yy);
};
const int FOV::MULT[8][4] = { {1,0,0,1},{0,1,1,0},{0,-1,1,0},{-1,0,0,1},{-1,0,0,-1},{0,-1,-1,-0},{0,1,-1,0},{1,0,0,-1} };
void FOV::compute(Map& map, int originX, int originY)
{
	for (int y = 0; y < MAP_HEIGHT;y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			map.tiles[y][x].visible = false;
		}
	}
	map.tiles[originY][originX].visible = true;
	map.tiles[originY][originX].explored = true;
	for (int oct = 0; oct < 8; oct++)
	{
		castLight(map, originX, originY, 1, 1.0f, 0.0f, MULT[oct][0], MULT[oct][1], MULT[oct][2], MULT[oct][3]);
	}
}