
#include <iostream>
#include <SDL.h>
#include "draw.h"
#include <chrono>
#include "vec2.h"
#include <fstream>
#include <random>
#include "Model.h"

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

float random() {
	return std::rand() % 1000 / 1000.0f;
}

static constexpr int ROWS = 91, COLS = 3;
static float trainData[ROWS][COLS] = {
	{ 0.0606, 0.1860, -0.22 },
	{ 0.0547, 0.1860, -0.8141  },
	{ 0.0543, 0.1860, -1.3509  },
	{ 0.0548, 0.1860, -1.8834  },
	{ 0.0538, 0.1860, -2.4213  },
	{ 0.0529, 0.1860, -2.9494  },
	{ 0.0529, 0.1860, -3.4682  },
	{ 0.0538, 0.1860, -3.9867  },
	{ 0.0529, 0.1860, -4.5148  },
	{ 0.0530, 0.1860, -5.0338  },
	{ 0.0534, 0.1860, -5.5538  },
	{ 0.0526, 0.1860, -6.0780  },
	{ 0.0580, 0.1860, -6.5938  },
	{ 0.0552, 0.1860, -7.1627  },
	{ 0.0535, 0.1860, -7.7045  },
	{ 0.0632, 0.1860, -8.2295  },
	{ 0.0534, 0.1860, -8.8493  },
	{ 0.0531, 0.1860, -9.3733  },
	{ 0.0533, 0.1860, -9.8941  },
	{ 0.0530, 0.1860, -10.4166 },
	{ 0.0553, 0.1860, -10.9363 },
	{ 0.0532, 0.1860, -11.4787 },
	{ 0.0531, 0.1860, -12.0010 },
	{ 0.0582, 0.1860, -12.5216 },
	{ 0.0524, 0.1860, -13.0926 },
	{ 0.0563, 0.1860, -13.6064 },
	{ 0.0526, 0.1860, -14.1589 },
	{ 0.0531, 0.1860, -14.6744 },
	{ 0.0569, 0.1860, -15.1948 },
	{ 0.0627, 0.1860, -15.7533 },
	{ 0.0536, 0.1860, -16.3679 },
	{ 0.0630, 0.1860, -16.8939 },
	{ 0.0533, 0.1860, -17.5116 },
	{ 0.0542, 0.1860, -18.0344 },
	{ 0.0602, 0.1860, -18.5662 },
	{ 0.0536, 0.1860, -19.1567 },
	{ 0.0533, 0.1860, -19.6825 },
	{ 0.0559, 0.1860, -20.2049 },
	{ 0.0574, 0.1860, -20.7532 },
	{ 0.0540, 0.1860, -21.3162 },
	{ 0.0553, 0.1860, -21.8460 },
	{ 0.0580, 0.1860, -22.3886 },
	{ 0.0559, 0.1860, -22.9576 },
	{ 0.0532, 0.1860, -23.5063 },
	{ 0.0595, 0.1860, -24.0281 },
	{ 0.0558, 0.1860, -24.6118 },
	{ 0.0578, 0.1860, -25.1593 },
	{ 0.0530, 0.1860, -25.7266 },
	{ 0.0529, 0.1860, -26.2466 },
	{ 0.0634, 0.1860, -26.7652 },
	{ 0.0529, 0.1860, -27.3874 },
	{ 0.0554, 0.1860, -27.9067 },
	{ 0.0633, 0.1860, -28.4497 },
	{ 0.0542, 0.1860, -29.0702 },
	{ 0.0539, 0.1860, -29.6024 },
	{ 0.0536, 0.1860, -30.1307 },
	{ 0.0538, 0.1860, -30.6561 },
	{ 0.0529, 0.1860, -31.1834 },
	{ 0.0549, 0.1860, -31.7019 },
	{ 0.0537, 0.1860, -32.2403 },
	{ 0.0530, 0.1860, -32.7672 },
	{ 0.0542, 0.1860, -33.2867 },
	{ 0.0543, 0.1860, -33.8187 },
	{ 0.0571, 0.1860, -34.3518 },
	{ 0.0563, 0.1860, -34.9119 },
	{ 0.0555, 0.1860, -35.4638 },
	{ 0.0544, 0.1860, -36.0082 },
	{ 0.0530, 0.1860, -36.5416 },
	{ 0.0528, 0.1860, -37.0617 },
	{ 0.0541, 0.1860, -37.5797 },
	{ 0.0528, 0.1860, -38.1103 },
	{ 0.0569, 0.1860, -38.6281 },
	{ 0.0547, 0.1860, -39.1866 },
	{ 0.0557, 0.1860, -39.7228 },
	{ 0.0568, 0.1860, -40.2692 },
	{ 0.0529, 0.1860, -40.8263 },
	{ 0.0528, 0.1860, -41.3449 },
	{ 0.0526, 0.1860, -41.8625 },
	{ 0.0543, 0.1860, -42.3789 },
	{ 0.0578, 0.1860, -42.9118 },
	{ 0.0549, 0.1860, -43.4787 },
	{ 0.0535, 0.1860, -44.0175 },
	{ 0.0530, 0.1860, -44.5421 },
	{ 0.0555, 0.1860, -45.0619 },
	{ 0.0643, 0.1860, -45.6066 },
	{ 0.0533, 0.1860, -46.2377 },
	{ 0.0548, 0.1860, -46.7608 },
	{ 0.0609, 0.1860, -47.2982 },
	{ 0.0542, 0.1860, -47.8956 },
	{ 0.0641, 0.1860, -48.4269 },
	{ 0.0557, 0.1860, -49.0562 }
};

int main(int argc, char* argv[]) {

	vec2 initial(1, 1);
	float dt = 0.05f;
	Model model;
	vec2 out = model.next(initial.x, initial.y, 0.05f);
	printf("Predicted %2.4f, %2.4f Expected %2.4f, %2.4f\n", out.x, out.y, initial.x, initial.y);
	printf("Cost: %f", model.cost(trainData, ROWS));

	return 0;

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

	point_mass p{ vec2(0, 0), vec2(random() * 6 - 3, random() * 20 - 10), 1.0f };

	std::ofstream dataFile;
	dataFile.open("data0.csv");

	auto last = std::chrono::high_resolution_clock::now();
	float t = 0.0f;
	while (running) {
		auto now = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration_cast<std::chrono::nanoseconds>(now - last).count()/1000000000.0f;
		last = now;

		t += dt;

		printf("%1.4f, %1.4f, %1.4f\n", dt, p.vel.x, p.vel.y);
		if (t >= 5)
			running = false;

		dataFile << dt << ", " << p.vel.x << ", " << p.vel.y << "\n";

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

		p.pos += p.vel * dt;
		p.vel += vec2(0, -g) * dt;

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