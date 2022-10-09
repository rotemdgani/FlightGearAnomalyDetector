/*
 * anomaly_detection_util.cpp
 *
 * Author: Nadav Vernia (ID 315535351) and Rotem Dgani (ID 208948927)
 */
#include <math.h>
#include "anomaly_detection_util.h"
#include <iostream>
using namespace std;

float avg(float* x, int size) {
		float sum = 0;
		for (int i = 0; i < size; i++) {
			sum += x[i];
		}
		return sum / size;
}

// returns the variance of X and Y
float var(float* x, int size) {
		float mu = pow((avg(x, size)),2);
		float sum_of_pow = 0;
		for (int i = 0; i < size; i++) {
			sum_of_pow += pow((x[i]), 2);
		}
		return (sum_of_pow / size) - mu;
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size) {
		float avg_x = avg(x, size);
		float avg_y = avg(y, size);
		float dev = 0;
		for (int i = 0; i < size; i++) {
			dev += ((x[i] - avg_x) * (y[i] - avg_y));
		}
		return dev / size;
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size) {
		float sigmaX = sqrt(var(x, size)); // squart of variance of X
		float sigmaY = sqrt(var(y, size)); // squart of variance of Y
		return ((cov(x, y, size)) / (sigmaX * sigmaY));
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size) {
	float* xValues = new float[size]; // array to store x values
	float* yValues = new float[size]; // array to store y values

	for (int i = 0; i < size; i++) { // going through the points array and fill the new arrays accordingly
		xValues[i] = points[i]->x;
		yValues[i] = points[i]->y;
	}

	float a = (cov(xValues, yValues, size)) / var(xValues, size);
	float b = avg(yValues, size) - (a * avg(xValues, size));

	delete[] xValues;
	delete[] yValues;
	Line result = Line(a, b); // creating the line from the points
	return result;
}

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point** points, int size) {
	Line lineFromPoints = linear_reg(points, size); // create the line equation
	return dev(p, lineFromPoints);
}

// returns the deviation between point p and the line
float dev(Point p, Line l) {
	double yValue = p.y;
	double expectedYValue = l.f(p.x); // calculate f(x) for given point
	double result = fabs(expectedYValue - yValue); // calculate absolut value of f(x) - y
	return static_cast<float>(result);
}