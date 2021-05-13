#pragma once
#include "vec2.h"

class Model
{
private:
	// weights: b0, w0
	float xWeights[2];
	float yWeights[2];
public:
	Model() {
		for (int i = 0; i < 2; i++) { xWeights[i] = 0; yWeights[i] = 0; }
		yWeights[1] = -9.81f;
	};

	void train(float** data, unsigned int iterations); // will train this model on the iterations

	vec2 next(float dt); // calculates the next velocity from the given inputs

	float cost(float evaluationData[][3], unsigned int n);
};

