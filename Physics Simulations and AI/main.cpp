
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

static constexpr int ROWS = 91, COLS = 3;
static float trainData[ROWS][COLS] = {
	{ 0.0591, 0.0000, -0.5795 },
	{ 0.0550, 0.0000, -0.5399 },
	{ 0.0546, 0.0000, -0.5357 },
	{ 0.0535, 0.0000, -0.5249 },
	{ 0.0539, 0.0000, -0.5283 },
	{ 0.0527, 0.0000, -0.5166 },
	{ 0.0529, 0.0000, -0.5187 },
	{ 0.0536, 0.0000, -0.5262 },
	{ 0.0528, 0.0000, -0.5180 },
	{ 0.0530, 0.0000, -0.5200 },
	{ 0.0528, 0.0000, -0.5182 },
	{ 0.0538, 0.0000, -0.5281 },
	{ 0.0558, 0.0000, -0.5476 },
	{ 0.0533, 0.0000, -0.5227 },
	{ 0.0525, 0.0000, -0.5152 },
	{ 0.0525, 0.0000, -0.5145 },
	{ 0.0532, 0.0000, -0.5221 },
	{ 0.0529, 0.0000, -0.5185 },
	{ 0.0529, 0.0000, -0.5191 },
	{ 0.0531, 0.0000, -0.5213 },
	{ 0.0538, 0.0000, -0.5273 },
	{ 0.0542, 0.0000, -0.5317 },
	{ 0.0632, 0.0000, -0.6204 },
	{ 0.0532, 0.0000, -0.5220 },
	{ 0.0524, 0.0000, -0.5136 },
	{ 0.0578, 0.0000, -0.5668 },
	{ 0.0621, 0.0000, -0.6093 },
	{ 0.0633, 0.0000, -0.6214 },
	{ 0.0587, 0.0000, -0.5758 },
	{ 0.0533, 0.0000, -0.5227 },
	{ 0.0548, 0.0000, -0.5376 },
	{ 0.0541, 0.0000, -0.5304 },
	{ 0.0585, 0.0000, -0.5744 },
	{ 0.0524, 0.0000, -0.5140 },
	{ 0.0576, 0.0000, -0.5649 },
	{ 0.0530, 0.0000, -0.5200 },
	{ 0.0527, 0.0000, -0.5167 },
	{ 0.0618, 0.0000, -0.6063 },
	{ 0.0625, 0.0000, -0.6133 },
	{ 0.0636, 0.0000, -0.6235 },
	{ 0.0532, 0.0000, -0.5215 },
	{ 0.0576, 0.0000, -0.5647 },
	{ 0.0529, 0.0000, -0.5192 },
	{ 0.0526, 0.0000, -0.5165 },
	{ 0.0532, 0.0000, -0.5218 },
	{ 0.0584, 0.0000, -0.5730 },
	{ 0.0624, 0.0000, -0.6125 },
	{ 0.0533, 0.0000, -0.5233 },
	{ 0.0527, 0.0000, -0.5170 },
	{ 0.0566, 0.0000, -0.5550 },
	{ 0.0546, 0.0000, -0.5359 },
	{ 0.0523, 0.0000, -0.5135 },
	{ 0.0548, 0.0000, -0.5371 },
	{ 0.0631, 0.0000, -0.6193 },
	{ 0.0527, 0.0000, -0.5173 },
	{ 0.0579, 0.0000, -0.5676 },
	{ 0.0525, 0.0000, -0.5148 },
	{ 0.0577, 0.0000, -0.5656 },
	{ 0.0550, 0.0000, -0.5400 },
	{ 0.0536, 0.0000, -0.5260 },
	{ 0.0534, 0.0000, -0.5236 },
	{ 0.0535, 0.0000, -0.5249 },
	{ 0.0532, 0.0000, -0.5215 },
	{ 0.0571, 0.0000, -0.5604 },
	{ 0.0627, 0.0000, -0.6148 },
	{ 0.0575, 0.0000, -0.5640 },
	{ 0.0538, 0.0000, -0.5279 },
	{ 0.0531, 0.0000, -0.5211 },
	{ 0.0627, 0.0000, -0.6150 },
	{ 0.0523, 0.0000, -0.5134 },
	{ 0.0533, 0.0000, -0.5233 },
	{ 0.0529, 0.0000, -0.5189 },
	{ 0.0547, 0.0000, -0.5361 },
	{ 0.0525, 0.0000, -0.5153 },
	{ 0.0528, 0.0000, -0.5184 },
	{ 0.0629, 0.0000, -0.6174 },
	{ 0.0525, 0.0000, -0.5148 },
	{ 0.0543, 0.0000, -0.5332 },
	{ 0.0527, 0.0000, -0.5170 },
	{ 0.0560, 0.0000, -0.5498 },
	{ 0.0525, 0.0000, -0.5155 },
	{ 0.0527, 0.0000, -0.5167 },
	{ 0.0591, 0.0000, -0.5796 },
	{ 0.0557, 0.0000, -0.5460 },
	{ 0.0524, 0.0000, -0.5141 },
	{ 0.0579, 0.0000, -0.5676 },
	{ 0.0532, 0.0000, -0.5218 },
	{ 0.0580, 0.0000, -0.5693 },
	{ 0.0624, 0.0000, -0.6121 },
	{ 0.0577, 0.0000, -0.5662 },
	{ 0.0596, 0.0000, -0.5843 },
};

int main(int argc, char* argv[]) {

	test();
	return 0;

	float dt = 0.05f;
	Model model;
	vec2 out = model.next(0.05f);
	printf("Predicted %2.4f, %2.4f Expected %2.4f, %2.4f\n", out.x, out.y, 0.0f, -g * dt);
	printf("Cost: %f\n", model.cost(trainData, ROWS));

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