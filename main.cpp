#include "SDL3_ttf/SDL_ttf.h"
#include "SDL3/SDL.h"
#include "map.h"
#include "GlyphCache.h"
#include "Player/Player.h"
static const char* FONT_PATH = "RobotoMono-Light.ttf";
static const float FONT_PH = 20.0f;
static const SDL_Color BG = { 12,12,16,255 };
static const SDL_Color WALL = { 130,120,150,255 };
static const SDL_Color FLOOR = { 72,66,60,255 };
int main(int argc,char* argv[])
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_Log("SDL_Init: %s", SDL_GetError());
		return 1;
	}
	if (!TTF_Init())
	{
		SDL_Log("TTF_Init: %s", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	SDL_Window* window = nullptr;
	SDL_Renderer* sdl = nullptr;
	if (!SDL_CreateWindowAndRenderer("Roguelike - part 1: Map Generation", MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE, 0, &window, &sdl))
	{
		SDL_Log("CreatWindowAndRenderer: %s", SDL_GetError());
		TTF_Quit();
		SDL_Quit();
		return 1;
	}
	GlyphCache glyphs(sdl, FONT_PATH, FONT_PH);
	if (!glyphs.ok())
	{
		SDL_DestroyRenderer(sdl);
		SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}
	Map map;
	Player player;
	auto newGame = [&]()
		{
			map.generate();
			player.x = map.rooms[0].centreX();
			player.y = map.rooms[0].centreY();
			player.hp = player.maxHp;
		};
	map.generate();
	static const SDL_Color PLAYER = { 255,230,150,255 };
	auto draw = [&]()
		{
			SDL_SetRenderDrawColor(sdl, BG.r, BG.g, BG.b, BG.a);
			SDL_RenderClear(sdl);
			for (int row = 0; row < MAP_HEIGHT; row++)
			{
				for (int col = 0; col < MAP_WIDTH; col++)
				{
					if (map.tiles[row][col].type == TileType::Floor)
					{
						glyphs.drawGlyph(row, col, '.', FLOOR);
					}
					else
					{
						glyphs.drawGlyph(row, col, '#', WALL);
					}
				}
			}
			glyphs.drawGlyph(player.x, player.y, '@', PLAYER);
			SDL_RenderPresent(sdl);
		};
	draw();
	bool running = true;
	SDL_Event event;
	while (running)
	{
		SDL_WaitEvent(&event);
		bool dirty = false;
		switch(event.type)
		{
		case SDL_EVENT_QUIT:
			running = false;
			break;
		case SDL_EVENT_WINDOW_EXPOSED:
			dirty = true;
			break;
		case SDL_EVENT_KEY_DOWN:
			int dx = 0;
			int dy = 0;
			switch (event.key.scancode)
			{
			case SDL_SCANCODE_UP:
			case SDL_SCANCODE_W:
				dy = -1;
				break;
			case SDL_SCANCODE_DOWN:
			case SDL_SCANCODE_S:
				dy = +1;
				break;
			case SDL_SCANCODE_LEFT:
			case SDL_SCANCODE_A:
				dx = -1;
				break;
			case SDL_SCANCODE_RIGHT:
			case SDL_SCANCODE_D:
				dx = +1;
				break;
			case SDL_SCANCODE_SPACE:
				newGame();
				dirty = true;
				break;
			case SDL_SCANCODE_ESCAPE:
				running = false;
				break;
			}
			if (dx != 0 || dy != 0)
			{
				dirty = player.tryMove(dx, dy, map);
				break;
			}
		}
		if (dirty)
		{
			draw();
		}
	}
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	return 0;
}