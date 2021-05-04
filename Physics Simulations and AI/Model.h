#pragma once
#include "vec2.h"

class Model
{
private:
	float xWeights[3];
	float yWeights[3];
public:
	Model() {
		for (int i = 0; i < 3; i++) { xWeights[i] = 0; yWeights[i] = 0; }
	};

	void train(float** data, unsigned int iterations); // will train this model on the iterations

	vec2 next(float xv, float yv, float dt); // calculates the next velocity from the given inputs

	float cost(float evaluationData[][3], unsigned int n);
};

