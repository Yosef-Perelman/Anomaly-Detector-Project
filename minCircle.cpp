#include "minCircle.h"

vector<Point> arrToVec(Point** points, size_t size){
    vector<Point> vec;
    for (int i = 0; i < size; i++)
    {
        vec.push_back(points[i][0]);
    }
    return vec;
}

float distance(const Point& a, const Point& b){
    return sqrt(pow(a.x - b.x,2) + pow(a.y - b.y,2));
}

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

Circle createCircleFromThree(const Point& a, const Point& b, const Point& c){
    Point center = return_circle_center(b.x - a.x, b.y - a.y, c.x - a.x, c.y - a.y);
    center.x += a.x;
    center.y += a.y;
    return { center, distance(center, a) };
}

Circle createCircleFromTwo(const Point& a, const Point& b){
    return {{(a.x + b.x / 2),(a.y + b.y / 2)}, distance(a, b) / 2};
}

// check for each point in the points set if it's in the circle
bool checkCircle(const Circle& circle, const vector<Point>& vec) {
    for (Point point : vec)
    {
        if(!checkPoint(point, circle)) {
            return false;
        }
    }
    return true;
}

Circle createCircle(const vector<Point>& vec){
   if(vec.size() == 0) {
       return {{0, 0}, 0};
   }
    else if(vec.size() == 1) {
        return {vec[0], 0};
    }
    else if(vec.size() == 2) {
        return createCircleFromTwo(vec[0], vec[1]);
    }
   else {
       for (int i = 0; i < 3; i++) {
           for (int j = i + 1; j < 3; j++) {

               Circle c = createCircleFromTwo(vec[i], vec[j]);
               if (checkCircle(c, vec))
                   return c;
           }
       }
   }
    // if not, create a circle by three points
    return createCircleFromThree(vec[0], vec[1], vec[2]);
}

// the main function of the algorithm. create the circle by recursion
Circle recursiveFunc(vector<Point>& vec, vector<Point> boundaries, size_t size)
{
    // the stop condition. if the size of the points vector is zero,
    // or the size of the boundaries vector is three
    if (size == 0 || boundaries.size() == 3) {
        return createCircle(boundaries);
    }

    Point point = vec[size];
    // get a random point from the vec and check if it's in the circle.
    // otherwise, it's on the boundaries of the circle, and call the functoin
    // recursively with the rest of the vector.
    Circle circle = recursiveFunc(vec, boundaries, size - 1);

    if (checkPoint(point, circle)) {
        return circle;
    }

    boundaries.push_back(point);

    return recursiveFunc(vec, boundaries, size - 1);
}

// implement
Circle findMinCircle(Point** points,size_t size){
    vector<Point> vec = arrToVec(points, size);
    return recursiveFunc(vec, {}, size);
}
