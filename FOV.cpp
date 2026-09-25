#pragma once
#include "map.h"
class FOV
{
public:
	static constexpr int RADIUS = 11;
	static void compute(Map& map, int originXint,int originY);
private:
	static const int MULT[8][4];
	static int castLight(Map& map, int cx, int cy, int row, float startSlope, float endSlope, int xx, int xy, int yx, int yy);
};