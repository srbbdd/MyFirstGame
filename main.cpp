#include "SDL3_ttf/SDL_ttf.h"
#include "SDL3/SDL.h"
#include "map.h"
#include "GlyphCache.h"
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
	map.generate();
}