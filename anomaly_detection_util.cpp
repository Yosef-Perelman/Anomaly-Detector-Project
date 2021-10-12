//
// Created by ariel on 12-Oct-21.
//

#include "anomaly_detection_util.h"
#include "anomaly_detection_util.h"
#include <cmath>

float anomaly_detection_util::dev(anomaly_detection_util::Point p, anomaly_detection_util::Point **points, int size) {
    Line l = linear_reg(points, size);
    return dev(p, l);
}

anomaly_detection_util::Line anomaly_detection_util::linear_reg(anomaly_detection_util::Point **points, int size) {
    float* xVal = new float [size];
    float* yVal = new float [size];

    for (int i = 0; i < size; ++i) {
        xVal[i] = points[i]->x;
        yVal[i] = points[i]->y;
    }
    double a = cov(xVal, yVal, size) / var(xVal, size);
    double b = mean(yVal, size) - a * mean(xVal, size);
    return anomaly_detection_util::Line(a, b);
}

float anomaly_detection_util::dev(anomaly_detection_util::Point p, anomaly_detection_util::Line l) {
    return std::fabs(p.x - l.f(p.x));
}

float anomaly_detection_util::pearson(float *x, float *y, int size) {
    return cov(x,y,size) / (sqrt(var(x, size)) * sqrt(var(y, size)));
}

float anomaly_detection_util::cov(float *x, float *y, int size) {
    float xMean = mean(x, size); float yMean = mean(y, size);
    float sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += (x[i] - xMean) * (y[i] - yMean);
    }
    return sum / size;
}

float anomaly_detection_util::var(float *x, int size) {
    float average = mean(x, size);
    float sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += pow(x[i] - average, 2);
    }
    return sum/size;
}

float anomaly_detection_util::mean(float *x, int size) {
    float sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += x[i];
    }
    return sum/size;
}