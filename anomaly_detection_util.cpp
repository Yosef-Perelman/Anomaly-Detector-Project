//
// Created by ariel on 11-Oct-21.
//

#include "anomaly_detection_util.h"
#include <cmath>

float anomaly_detection_util::dev(anomaly_detection_util::Point p, anomaly_detection_util::Point **points, int size) {
    return 0;
}

anomaly_detection_util::Line anomaly_detection_util::linear_reg(anomaly_detection_util::Point **points, int size) {
    float [size]
    double a = points[0]->x;
    return anomaly_detection_util::Line();
}

float anomaly_detection_util::dev(anomaly_detection_util::Point p, anomaly_detection_util::Line l) {
    return 0;
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