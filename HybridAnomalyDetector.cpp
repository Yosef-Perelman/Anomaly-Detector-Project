/*
 * HybridAnomalyDetector.cpp
 *
 * Author: Yosef Perelman 206344814 and Ariel Mantel 313450249
 */

#include "HybridAnomalyDetector.h"

const float highThreshHold = 0.9;
const float lowThreshHold = 0.5;

HybridAnomalyDetector::HybridAnomalyDetector() {
	// TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

/*
 * Set the threshHold by the farthest point from the center of the circle.
 */
void HybridAnomalyDetector::setThreshHold(correlatedFeatures& corrfea, Point** pointsArr, int i) {
    // if (corrfea.corrlation < highThreshHold) {
    corrfea.circle = findMinCircle(pointsArr, i);
    for (int j = 0; j < i; ++j) {
        Point p(pointsArr[j]->x, pointsArr[j]->y);
        float dist = distance(p, corrfea.circle.center);
        if (dist > corrfea.threshold) {
            corrfea.threshold = dist;
        }
    }
}

/*
 * Check anomaly.
 * If correlation >= 0.9 so check if the distance between the point and the regresion line is bigger than the threshHold.
 * Otherwise, check if the distance between the point and the circle center is bigger than the threshHold.
 */
int HybridAnomalyDetector::checkAnomaly(const TimeSeries& ts, const correlatedFeatures& corrfea, int i){
    // Creates point from the values in the data vector (vector[i]->x and vector[i]->y).
    Point p(ts.dataMap.find(corrfea.feature1)->second[i], ts.dataMap.find(corrfea.feature2)->second[i]);
    // If the correlation < 0.9
    if (corrfea.corrlation < highThreshHold){
        if (distance(corrfea.circle.center, p) > (corrfea.threshold * 1.1))
            return 1;
        else
            return 0;
    }
    // Otherwise
    else{
        if(std::abs(ts.dataMap.find(corrfea.feature2)->second[i] - (ts.dataMap.find(corrfea.feature1)->second[i]
        * corrfea.lin_reg.a + corrfea.lin_reg.b)) > (corrfea.threshold * 1.1))
            return 1;
        else
            return 0;
    }
}

