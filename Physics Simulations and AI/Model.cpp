#include "Model.h"

vec2 Model::next(float xv, float yv, float dt)
{
	vec2 accum;
	for (int i = 1; i < 3; i++) {
		accum += vec2(xWeights[i], yWeights[i]);
	}
	return accum * dt + vec2(xWeights[0], yWeights[0]);
}

float Model::cost(float evaluationData[][3], unsigned int n) 
{
	float cost = 0.0f;
	vec2 vel(evaluationData[0][1], evaluationData[0][2]);
	for (int i = 0; i < n - 1; i++) {
		vec2 nextVelocity = next(vel.x, vel.y, evaluationData[i][0]);
		float dx = nextVelocity.x - evaluationData[i + 1][1];
		float dy = nextVelocity.y - evaluationData[i + 1][2];
		cost += dx * dx + dy * dy;
	}
	return cost / (n - 1);
}
