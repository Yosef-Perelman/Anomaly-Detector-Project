/*
 * anomaly_detection_util.cpp
 *
 * Author: Yosef Perelman 206344814
 */

#include <cmath>
#include "anomaly_detection_util.h"

float avg(float* x, int size){
    float avg = 0;
    for (int i = 0; i < size; i++) {
        avg += x[i];
    }
    return avg / size;;
}

// returns the variance of X and Y
float var(float* x, int size){
    float avg1 = 0;
    for (int i = 0; i < size; i++) {
        avg1 += pow(x[i], 2.0);
    }
    avg1 = avg1  / size;
    return avg1 - pow(avg(x, size), 2.0);
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size){
    float avg1 = 0;
    for (int i = 0; i < size; i++) {
        avg1 += (x[i] * y[i]);
    }
    avg1 = avg1  / size;
    return avg1 - (avg(x, size) * avg(y, size));
}

// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size){
    return cov(x, y, size) / (sqrt(var(x, size)) * sqrt(var(y, size)));
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size){
    float x[size];
    float y[size];
    for (int i = 0; i < size; i++)
    {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    return Line(cov(x, y, size) / var(x, size), avg(y, size) - (cov(x, y, size) / var(x, size) * avg(x, size)));
}

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point** points, int size){
    Line l = linear_reg(points, size);
    return std::abs((p.x * l.a + l.b) - p.y);
}

// returns the deviation between point p and the line
float dev(Point p, Line l){
    return std::abs((p.x * l.a + l.b) - p.y);
}



