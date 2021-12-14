/*
 * minCircle.cpp
 *
 * Author: Yosef Perelman 206344814 and Ariel Mantel 313450249
 */

#include "minCircle.h"

/*
 * Converts 2D array of points to vector of points.
 */
 vector<Point> arrToVec(Point** points, size_t size){
    vector<Point> vec;
    for (int i = 0; i < size; i++)
    {
        vec.push_back(points[i][0]);
    }
    return vec;
}

/*
 * Get two points and returns the distance between them.
 */
float distance(const Point& a, const Point& b){
    return float(sqrt(pow(a.x - b.x,2) + pow(a.y - b.y,2)));
}

/*
 * Get point and circle and checks if the point is in the circle.
 * Actually check if the distance between the point and the center of the circle is less or equal to the radius.
 */
bool checkPoint(const Point& point, const Circle& circle){
    return distance(circle.center, point) <= circle.radius;
}

Point return_circle_center(float ax, float ay, float bx, float by){
    float A = ax * ax + ay * ay;
    float B = bx * bx + by * by;
    float C = ax * by - ay * bx;
    return { (by * A - ay * B) / (2 * C),
             (ax * B - bx * A) / (2 * C) };
}

/*
 * Creates circle by three points.
 */
Circle createCircleFromThree(const Point& a, const Point& b, const Point& c){
    Point center = return_circle_center(b.x - a.x, b.y - a.y, c.x - a.x, c.y - a.y);
    center.x += a.x;
    center.y += a.y;
    return { center, distance(center, a) };
}

/*
 * Gets two points and returns the circle that they create together.
 * Actually the circle center is the point in the middle of the points,
 * and the radius is the distance between them / 2.
 */
Circle createCircleFromTwo(const Point& a, const Point& b){
    Point center = {float((a.x + b.x) / 2.0), float((a.y + b.y) / 2.0)};
    return {center, float(distance(a, b) / 2.0)};
}

/*
 * Gets circle and vector of points and check for each point if it in the circle with the checkPoint method.
 */
bool checkCircle(const Circle& circle, const vector<Point>& vec) {
    for (Point point : vec)
    {
        if(!checkPoint(point, circle)) {
            return false;
        }
    }
    return true;
}

/*
 * Gets vector of three or fewer points, and creates circle from the point.
 */
Circle createCircle(const vector<Point>& vec){
   if(vec.empty()) {
       return {{0, 0}, 0};
   }
   else if(vec.size() == 1) {
        return {vec[0], 0};
    }
   else if(vec.size() == 2) {
        return createCircleFromTwo(vec[0], vec[1]);
    }
    // If their three point, first tries to create circle from two of the points
    // and check if the third point is in the circle. If it didn't work with any pair of the points,
    // create circle from all the three (it means they all on the boundaries of the circle).
   else {
       for (int i = 0; i < 3; i++) {
           for (int j = i + 1; j < 3; j++) {

               Circle circle = createCircleFromTwo(vec[i], vec[j]);
               if (checkCircle(circle, vec))
                   return circle;
           }
       }
   }
    return createCircleFromThree(vec[0], vec[1], vec[2]);
}

/*
 * Get vector of points and creates circle by recursion.
 * In every call of the method the vector has less one point, until it get to the base case where there three points
 * or the size of the boundaries vector that keep all the points that sit on the boundary of the circle is equal
 * to three.
 */
Circle recursiveFunc(vector<Point>& vec, vector<Point> boundaries, size_t size)
{
    if (size == 0 || boundaries.size() == 3) {
        return createCircle(boundaries);
    }

    // Pick a random point randomly
    int idx = rand() % size;
    Point point = vec[idx];

    // Put the picked point at the end of P
    // since it's more efficient than
    // deleting from the middle of the vector
    swap(vec[idx], vec[size - 1]);

    Circle circle = recursiveFunc(vec, boundaries, size - 1);

    if (checkPoint(point, circle)) {
        return circle;
    }

    boundaries.push_back(point);

    return recursiveFunc(vec, boundaries, size - 1);
}

/*
 * Implementation.
 */
Circle findMinCircle(Point** points,size_t size){
    vector<Point> vec = arrToVec(points, size);
    return recursiveFunc(vec, {}, size);
}
