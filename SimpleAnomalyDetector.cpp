/*
 * anomaly_detection_util.cpp
 *
 * Author: Yosef Perelman 206344814 and Ariel Mantel 313450249
 */

#include "SimpleAnomalyDetector.h"

const float threshHold = 0.9;

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    // TODO Auto-generated constructor stub

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
    //Vector with the features names
    vector<string> featuresNames = ts.getFeaturesName();
    string feature1Name, feature2Name;
    int colNum = ts.getColNum(), dataSize = ts.getLineNum();
    // TODO initial the arrays dynamically
    float infoArr1[dataSize], infoArr2[dataSize], temp[dataSize];
    //Move on all the feature except the last
    for (int i = 0; i < colNum; ++i) {
        int c = -1;
        float correlation = 0;
        feature1Name = featuresNames[i];
        copy(ts.dataMap.find(feature1Name)->second.begin(), ts.dataMap.find(feature1Name)->second.end(), infoArr1);
        for (int j = i + 1; j < colNum; ++j) {
            string tempName = featuresNames[j];
            copy(ts.dataMap.find(tempName)->second.begin(), ts.dataMap.find(tempName)->second.end(), temp);
            if (std::abs(pearson(infoArr1, temp, dataSize)) > correlation) {
                correlation = std::abs(pearson(infoArr1, temp, dataSize));
                feature2Name = tempName;
                c = j;
            }
        }
        if (c != -1) {
            if (correlation > threshHold) {
                copy(ts.dataMap.find(feature2Name)->second.begin(), ts.dataMap.find(feature2Name)->second.end(),
                     infoArr2);
                Point* pointsArr[dataSize];
                for (int i = 0; i < dataSize; ++i) {
                    pointsArr[i] = new Point(infoArr1[i], infoArr2[i]);
                }
                correlatedFeatures corrfea;
                corrfea.feature1 = feature1Name;
                corrfea.feature2 = feature2Name;
                corrfea.corrlation = std::abs(pearson(infoArr1, infoArr2, dataSize));
                corrfea.lin_reg = linear_reg(pointsArr, dataSize);
                corrfea.threshold = 0;
                for (int j = 0; j < dataSize; ++j) {
                    if (dev(*pointsArr[j], corrfea.lin_reg) > corrfea.threshold) {
                        corrfea.threshold = dev(*pointsArr[j], corrfea.lin_reg);
                    }
                }
                cf.push_back(corrfea);
                for (int j = 0; j < dataSize; ++j) {
                    delete pointsArr[j];
                }
            }
        }
    }
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    int dataSize = ts.getLineNum();
    for (int i = 0; i < cf.size(); ++i) {
        for (int j = 0; j < dataSize; ++j) {
            if (std::abs(ts.dataMap.find(cf[i].feature2)->second[j] - (ts.dataMap.find(cf[i].feature1)->second[j]
            * cf[i].lin_reg.a + cf[i].lin_reg.b)) > (cf[i].threshold * 1.1)) {
                AnomalyReport anomalyReport = AnomalyReport(cf[i].feature1+"-"+cf[i].feature2, j+1);
                ar.push_back(anomalyReport);
            }
        }
    }
    return ar;
}
