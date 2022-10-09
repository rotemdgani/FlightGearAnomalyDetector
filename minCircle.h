
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"

using namespace std;


// ------------ DO NOT CHANGE -----------

class Circle{
public:
	Point center;
	float radius;
	Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------

Circle findMinCircle(Point** points,size_t size);
float twoPointsDistance(Point a, Point b);
Circle circleFrom2Points(Point a, Point b);
Circle circleFrom3Points(Point a, Point b, Point c);
bool isPointInCircle(Circle circle, Point point);
Circle findMinCircleProcess(Point** points, size_t size, vector<Point> R);

// you can add here additional methods

#endif /* MINCIRCLE_H_ */
