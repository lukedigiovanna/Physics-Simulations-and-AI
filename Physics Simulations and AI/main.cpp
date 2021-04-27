
#include <iostream>
#include <SDL.h>
#include "draw.h"
#include <chrono>
#include "vec2.h"

static bool running = true;

#define g 9.81f

static vec2 camSize{ 10.0f, 10.0f };
static vec2 camPos{ 0.0f, 0.0f };
static const vec2 displaySize{ 640, 480 };

static vec2 lastMousePosition;

static bool mouseDown = false;

typedef struct {
	vec2 pos;
	vec2 vel;
	float size;
} point_mass;

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("err init sdl\n");
		return -1;
	}

	SDL_Window* window = SDL_CreateWindow("Free Fall Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Texture* display = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)displaySize.x, (int)displaySize.y);

	draw::setRenderer(renderer);

	SDL_Event e;

	point_mass p{ vec2(-3, 0), vec2(2, 8), 1.0f };

	auto last = std::chrono::high_resolution_clock::now();
	float t = 0.0f;
	while (running) {
		auto now = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration_cast<std::chrono::nanoseconds>(now - last).count()/1000000000.0f;
		last = now;

		t += dt;

	//	printf("%f\n", t);

		//if (t >= 10) {
		//	running = false;
		//	break;
	//	}

		int winWidth, winHeight;
		SDL_GetWindowSize(window, &winWidth, &winHeight);
		vec2 winSize((float)winWidth, (float)winHeight);

		while (SDL_PollEvent(&e) != NULL) {
			switch (e.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_MOUSEMOTION:
				if (mouseDown)
					camPos -= (vec2((float)e.motion.x, (float)e.motion.y) - lastMousePosition) / winSize * camSize * vec2(1, -1);
				lastMousePosition.x = (float)e.motion.x;
				lastMousePosition.y = (float)e.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				mouseDown = true;
				break;
			case SDL_MOUSEBUTTONUP:
				mouseDown = false;
				break;
			case SDL_MOUSEWHEEL:
				camSize *= (1 + 0.1 * e.wheel.y);
				break;

			}
		}

		p.pos += p.vel * dt;
		p.vel += vec2(0, -g) * dt;

		SDL_SetRenderTarget(renderer, display);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		draw::setColor(255, 125, 125);
		
		vec2 camBottomLeft = (camPos - camSize / 2.0f);
		vec2 screenPos = (p.pos - camBottomLeft) / camSize * displaySize;
		draw::fillCircle(screenPos.x, displaySize.y - screenPos.y, p.size / camSize.x * displaySize.x);

		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderCopy(renderer, display, NULL, NULL);
		SDL_RenderPresent(renderer);

		SDL_Delay(1);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}