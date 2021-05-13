#include "Model.h"

vec2 Model::next(float dt)
{
	vec2 accum;
	accum += vec2(xWeights[0], yWeights[0]);
	accum += vec2(xWeights[1], yWeights[1]) * dt;
	return accum;
}

float Model::cost(float evaluationData[][3], unsigned int n) 
{
	float cost = 0.0f;
	vec2 expected(evaluationData[0][1], evaluationData[0][2]);
	for (int i = 0; i < n; i++) {
		vec2 predicted = next(evaluationData[i][0]);
		float dx = expected.x - predicted.x;
		float dy = expected.y - predicted.y;
		cost += (expected - predicted).squaredMagnitude();
	}
	return cost / n;
}
