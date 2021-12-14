/*
 * HybridAnomalyDetector.h
 *
 * Author: Yosef Perelman 206344814 and Ariel Mantel 313450249
 */

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();

    void setThreshHold(correlatedFeatures& corrfea, Point** pointsArr, int i) override;
    int checkAnomaly(const TimeSeries& ts, const correlatedFeatures& corrfea, int i) override;
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
