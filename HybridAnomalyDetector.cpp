
#include "HybridAnomalyDetector.h"

const float highThreshHold = 0.9;
const float lowThreshHold = 0.5;

HybridAnomalyDetector::HybridAnomalyDetector() {
	// TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

void HybridAnomalyDetector::setThreshHold(correlatedFeatures& corrfea, Point** pointsArr, int i){
    if (corrfea.corrlation < highThreshHold) {
        corrfea.circle = findMinCircle(pointsArr, i);
        for (int j = 0; j < i; ++j) {
            Point p(pointsArr[j]->x, pointsArr[j]->y);
            float dist = distance(p, corrfea.circle.center);
            if (dist > corrfea.threshold) {
                corrfea.threshold = dist;
            }
        }
    }
    else {
        for (int j = 0; j < i; ++j) {
            Point p(pointsArr[j]->x, pointsArr[j]->y);
            float dist = dev(p, corrfea.lin_reg);
            if (dist > corrfea.threshold) {
                corrfea.threshold = dist;
            }
        }
    }
}

int HybridAnomalyDetector::checkAnomaly(const TimeSeries& ts, const correlatedFeatures& corrfea, int i){
    Point p(ts.dataMap.find(corrfea.feature1)->second[i], ts.dataMap.find(corrfea.feature2)->second[i]);
    if (corrfea.corrlation < highThreshHold){
        if (distance(corrfea.circle.center, p) > (corrfea.threshold * 1.1))
            return 1;
        else
            return 0;
    }
    else{
        if(std::abs(ts.dataMap.find(corrfea.feature2)->second[i] - (ts.dataMap.find(corrfea.feature1)->second[i]
        * corrfea.lin_reg.a + corrfea.lin_reg.b)) > (corrfea.threshold * 1.1))
            return 1;
        else
            return 0;
    }
}

