#include "linreg.h"

#include <iostream>

void LeastSquaresRegression::fit(float data[][2], int n) {
	const float lr = 0.01f; // learning rate
	for (int k = 0; k < 1000; k++) {
		// calculate the current gradient
		float grIntercept = 0.0f, grSlope = 0.0f;
		for (int i = 0; i < n; i++) {
			grIntercept += predict(data[i][0]) - data[i][1];
			grSlope += (predict(data[i][0]) - data[i][1]) * data[i][0];
		}
		printf("gradient = <%f, %f> y = %f + %fx cost = %f\n", grIntercept, grSlope, intercept, slope, getCost(data, n));
		slope -= lr * grSlope;
		intercept -= lr * grIntercept;
	}
}

float LeastSquaresRegression::predict(float x) const {
	return intercept + x * slope;
}

float LeastSquaresRegression::getCost(float data[][2], int n) {
	float cost = 0;
	for (int i = 0; i < n; i++) {
		float residual = predict(data[i][0]) - data[i][1];
		cost += residual * residual;
	}
	return cost;
}
