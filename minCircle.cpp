/*
 * minCircle.cpp
 *
 * Author: Nadav Vernia (ID 315535351) and Rotem Dgani (ID 208948927)
 */
#include "minCircle.h"
#include <math.h>

// calculate distance between 2 points
float twoPointsDistance(Point a, Point b) {
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

// create and return Circle object from 2 points
Circle circleFrom2Points(Point a, Point b) {
	//float xCenterPoint = (a.x + b.x) / 2; // this is the middle between x values of the points so it'll be the x value of the circle center point
	//float yCenterPoint = (a.y + b.y) / 2; // this is the middle between y values of the points so it'll be the y value of the circle center point
	//float radius = twoPointsDistance(a, b) / 2; // the distance between the points is the diameter, so divided by 2 is the radius
	return Circle(Point((a.x + b.x) / 2, (a.y + b.y) / 2), twoPointsDistance(a, b) / 2);
}

// create and return Circle object from 3 points, by formulas
Circle circleFrom3Points(Point a, Point b, Point c) {
	float x1 = a.x;
	float x2 = b.x;
	float x3 = c.x;
	float y1 = a.y;
	float y2 = b.y;
	float y3 = c.y;
	float pow1 = pow(x1, 2) + pow(y1, 2);
	float pow2 = pow(x2, 2) + pow(y2, 2);
	float pow3 = pow(x3, 2) + pow(y3, 2);
	float detA = x1 * (y2 - y3) - y1 * (x2 - x3) + (x2 * y3) - (x3 * y2);
	float detB = pow1 * (y3 - y2) + pow2 * (y1 - y3) + pow3 * (y2 - y1);
	float detC = pow1 * (x2 - x3) + pow2 * (x3 - x1) + pow3 * (x1 - x2);
	float detD = pow1 * ((x3 * y2) - (x2 * y3)) + pow2 * ((x1 * y3) - (x3 * y1)) + pow3 * ((x2 * y1) - (x1 * y2));
	//float x = (-1) * (detB / (2 * detA)); // the x value of the circle center point
	//float y = (-1) * (detC / (2 * detA)); // the y value of the circle center point
	//float radius = sqrt(pow((x - x1), 2) + pow((y - y1), 2)); // the radius of the circle
	Point center = Point(((-1) * (detB / (2 * detA))), ((-1) * (detC / (2 * detA))));
	return Circle(center, (sqrt(pow((center.x - x1), 2) + pow((center.y - y1), 2))));
}

// checks if a given point is indise the circle, so that means that it isn't on the boundary of the circle
bool isPointInCircle(Circle circle, Point point) {
	if (twoPointsDistance(circle.center, point) <= circle.radius) { // if the distance is equal or less than the radius then the point is inside
		return true;
	}
	return false;
}

// we implemented the Welzl's recursive algorithm that we find on literature, we will explain each stage.
// this function recieves set of points, size of input, vector R that is at first empty, and the main idea
//is to detect 1,2 or 3 points that located on the boundary of the circle
//the function recursivly remove 1 of the points from the input, randomly, and called again without the removed point in order to detect the
// points (if exist) that are lays on the boundary of the circle and which help find the minimum curcle
Circle findMinCircleProcess(Point** points, size_t size, vector<Point> R) {

	if (R.size() == 3) { // that means that we found 3 points from which we can create minimal circle
		return circleFrom3Points(R[0], R[1], R[2]);
	}
	if (size == 0) { // that means we went through all the points 
		if (R.size() == 0) {  // that means that we finished to go through all the points from input and none of them were on the boundary
			return Circle(Point(0, 0), 0);
		}
		else if (R.size() == 1) { // that means that we finished to go through all the points from input and only 1 of them were on the boundary
			return Circle(R[0], 0);
		}
		//else if (R.size() == 2) { // that means that we finished to go through all the points from input and only 2 of them were on the boundary
		return circleFrom2Points(R[0], R[1]);
	}

	Point suspect = *points[size - 1]; // the point to exclude from the points set

	Circle circle = findMinCircleProcess(points, (size - 1), R); // reduce the size by 1
	if (isPointInCircle(circle, suspect)) { // check if the point we removed is in the circle - if yes return the circle, else that means that
		//this point is on the boundary, so she will be added to vector R
		return circle;
	}

	R.push_back(suspect); //add the point to vector R that represent the points that has been found to lay on the boundary

	return findMinCircleProcess(points, (size - 1), R);
}
Circle findMinCircle(Point** points, size_t size) {
	return findMinCircleProcess(points, size, {});
}