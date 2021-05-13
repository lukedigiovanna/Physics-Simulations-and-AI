
#include <iostream>
#include <SDL.h>
#include "draw.h"
#include <chrono>
#include "vec2.h"
#include <fstream>
#include <random>
#include "Model.h"
#include "linreg.h"

static bool running = true;

#define g 9.81f

static vec2 camSize{ 10.0f, 10.0f };
static vec2 camPos{ 0.0f, 0.0f };
static const vec2 displaySize{ 640, 480 };

static vec2 lastMousePosition;

static bool mouseDown = false;

typedef struct {
	vec2 pos; // current position
	vec2 vel; // current velocity
	vec2 pVel; // previous velocity
	float size; // size
} point_mass;

float random() {
	return std::rand() % 1000 / 1000.0f;
}

static constexpr int ROWS = 6, COLS = 2;
static float trainData[ROWS][COLS] = {
	{ 0, 5 },
	{ 1, 5 },
    { 2, 5 },
	{ 3, 5 },
	{ 4, 5 },
	{ 5, 5 }
};

int main(int argc, char* argv[]) {

	LeastSquaresRegression regression;
	regression.fit(trainData, ROWS);

	return 0;

	/*
	float dt = 0.05f;
	Model model;
	vec2 out = model.next(0.05f);
	printf("Predicted %2.4f, %2.4f Expected %2.4f, %2.4f\n", out.x, out.y, 0.0f, -g * dt);
	printf("Cost: %f\n", model.cost(trainData, ROWS));
	*/

	//return 0;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("err init sdl\n");
		return -1;
	}

	std::srand(time(NULL));

	SDL_Window* window = SDL_CreateWindow("Free Fall Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Texture* display = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)displaySize.x, (int)displaySize.y);

	draw::setRenderer(renderer);

	SDL_Event e;

	vec2 initVelocity = vec2(random() * 6 - 3, random() * 20 - 10);
	point_mass p{ vec2(0, 0), initVelocity, initVelocity, 1.0f };

	std::ofstream dataFile;
	dataFile.open("data0.csv");

	auto last = std::chrono::high_resolution_clock::now();
	float t = 0.0f;
	while (running) {
		auto now = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration_cast<std::chrono::nanoseconds>(now - last).count()/1000000000.0f;
		last = now;

		t += dt;

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
				camSize *= (1 - 0.1 * e.wheel.y);
				break;

			}
		}

		p.pVel = p.vel; // copy the velocity into the previous velocity
		p.vel += vec2(0, -g) * dt; // apply the gravity vector to the particle
		p.pos += p.vel * dt;

		// write the data
		printf("%1.4f, %1.4f, %1.4f\n", dt, (p.vel.x - p.pVel.x), (p.vel.y - p.pVel.y));
		if (t >= 5)
			running = false;

		dataFile << dt << ", " << (p.vel.x - p.pVel.x) << ", " << (p.vel.y - p.pVel.y) << "\n";

		SDL_SetRenderTarget(renderer, display);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		draw::setColor(7, 111, 222);
		
		vec2 camBottomLeft = (camPos - camSize / 2.0f);
		vec2 screenPos = (p.pos - camBottomLeft) / camSize * displaySize;
		draw::fillCircle(screenPos.x, displaySize.y - screenPos.y, p.size / camSize.x * displaySize.x);

		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderCopy(renderer, display, NULL, NULL);
		SDL_RenderPresent(renderer);

		SDL_Delay(50);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	dataFile.close();

	return 0;
}