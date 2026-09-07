#pragma once
#include <SDL3/SDL.h>
class GlyphCache
{
public:
	GlyphCache(SDL_Renderer* sdl, const char* fontPath, float ptSize);
	~GlyphCache();
	bool ok()const
	{
		return m_loaded;
	}
	void drawGlyph(int col, int row, char ch, SDL_Color color)const;
private:
	SDL_Renderer* m_sdl= nullptr;
	bool m_loaded = false;
	static constexpr int FIRST_CHAR = 32;
	static constexpr int LAST_CHAR = 126;
	static constexpr int NUM_CHARS = LAST_CHAR - FIRST_CHAR + 1;
	struct Glyph
	{
		SDL_Texture* tex = nullptr;
		int w = 0;
		int h = 0;
	};
	Glyph m_glyphs[NUM_CHARS];
};