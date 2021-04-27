#include "draw.h"

void draw::setRenderer(SDL_Renderer* renderer)
{
	draw::renderer = renderer;
}

void draw::setColor(Uint8 r, Uint8 g, Uint8 b)
{
	SDL_SetRenderDrawColor(draw::renderer, r, g, b, 255);
}

void draw::setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(draw::renderer, r, g, b, a);
}

void draw::drawPixel(float x, float y)
{
	SDL_RenderDrawPoint(draw::renderer, (int)x, (int)y);
}

void draw::fillCircle(float x, float y, float radius)
{
	float r2 = radius * radius;
	for (float dx = -radius; dx <= 0; dx++)
		for (float dy = -radius; dy <= 0; dy++) 
			if (dx * dx + dy * dy < r2) {
				drawPixel(x + dx, y + dy);
				drawPixel(x - dx, y + dy);
				drawPixel(x + dx, y - dy);
				drawPixel(x - dx, y - dy);
			}
}
