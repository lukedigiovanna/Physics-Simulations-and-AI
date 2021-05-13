#pragma once

class LeastSquaresRegression {
private:
	float slope, intercept;
public:
	LeastSquaresRegression() : slope(0), intercept(0) {}

	void fit(float data[][2], int n); // fits the model to the data

	float predict(float x) const;

	float getCost(float data[][2], int n);
};