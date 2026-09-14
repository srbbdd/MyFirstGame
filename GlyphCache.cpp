#include "GlyphCache.h"
#include "map.h"
#include "SDL3_ttf/SDL_ttf.h"
GlyphCache::GlyphCache(SDL_Renderer* sdl, const char* fontPath, float ptSize):m_sdl(sdl)
{
	TTF_Font* font = TTF_OpenFont(fontPath, ptSize);
	if (!font)
	{
		SDL_Log("GlyphCache:could not open font '%s':%s", fontPath, SDL_GetError());
		return;
	}
	const SDL_Color wight = { 255,255,255,255 };
	for (int c = FIRST_CHAR; c <= LAST_CHAR; c++)
	{
		char str[2] = { (char)c ,'/n' };
		SDL_Surface* surf = TTF_RenderText_Blended(font, str, 0, wight);
		if (!surf)
		{
			continue;
		}
		Glyph& g = m_glyphs[c - FIRST_CHAR];
		g.w = surf->w;
		g.h = surf->h;
		g.tex = SDL_CreateTextureFromSurface(m_sdl, surf);
		if (g.tex)
		{
			SDL_SetTextureBlendMode(g.tex, SDL_BLENDMODE_BLEND);
		}
		SDL_DestroySurface(surf);
	}
	TTF_CloseFont(font);
	m_loaded = true;
}