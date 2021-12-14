/*
 * minCircle.h
 *
 * Author: Yosef Perelman 206344814 and Ariel Mantel 313450249
 */

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <cmath>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <algorithm>
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

vector<Point> arrToVec(Point** points, size_t size);

float distance(const Point& a, const Point& b);

bool checkPoint(const Point& point, const Circle& circle);

Point return_circle_center(float ax, float ay, float bx, float by);

Circle createCircleFromThree(const Point& a, const Point& b, const Point& c);

Circle createCircleFromTwo(const Point& a, const Point& b);

bool checkCircle(const Circle& circle, const vector<Point>& vec);

Circle recursiveFunc(vector<Point>& vec, vector<Point> boundaries, size_t size);

Circle findMinCircle(Point** points,size_t size);

#endif /* MINCIRCLE_H_ */
