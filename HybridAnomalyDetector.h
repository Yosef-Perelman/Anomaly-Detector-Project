

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

    virtual void setThreshHold(correlatedFeatures& corrfea, Point** pointsArr, int i);
    virtual int checkAnomaly(const TimeSeries& ts, const correlatedFeatures& corrfea, int i);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
