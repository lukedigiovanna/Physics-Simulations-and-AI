#pragma once

#include <SDL_render.h>

namespace draw {
	static SDL_Renderer* renderer;

	void setRenderer(SDL_Renderer* renderer);

	void setColor(Uint8 r, Uint8 g, Uint8 b);
	void setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	void drawPixel(float x, float y);
	void fillCircle(float x, float y, float radius);
}