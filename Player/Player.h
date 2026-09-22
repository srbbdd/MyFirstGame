#pragma once
#include "..\map.h"
struct Player
{
	int x = 0;
	int y = 0;
	int hp = 30;
	int maxHp = 30;
	bool tryMove(int dx, int dy, const Map& map)
	{
		int nx = x + dx;
		int ny = y + dy;
		if (nx < 0 || nx >= MAP_WIDTH)
		{
			return false;
		}
		if (ny < 0 || ny >= MAP_HEIGHT)
		{
			return false;
		}
		if (map.tiles[ny][nx].type != TileType::Floor)
		{
			return false;
		}
		x = nx;
		y = ny;
		return true;
	}
};