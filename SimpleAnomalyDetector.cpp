/*
 * anomaly_detection_util.cpp
 *
 * Author: Yosef Perelman 206344814 and Ariel Mantel 313450249
 */

#include "SimpleAnomalyDetector.h"

const float highThreshHold = 0.9;
const float lowThreshHold = 0.5;

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    // TODO Auto-generated constructor stub

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

void SimpleAnomalyDetector::setCorrelationTheresHold(float th) {correlationThreshHold = th;}

/*
 * Check for correlations between the features according to the input.
 */
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {

    vector<string> featuresNames = ts.getFeaturesName(); // The features name
    string feature1Name, feature2Name;
    int colNum = ts.getColNum(), dataSize = ts.getLineNum(); // The size of the data
    float infoArr1[dataSize], infoArr2[dataSize], temp[dataSize]; // Arrays to save the information

    // Goes through all the features from the first and checks a correlation between them
    // and the other features that follow.
    for (int i = 0; i < colNum; ++i) {
        int flag = -1;
        float correlation = 0;
        feature1Name = featuresNames[i];
        copy(ts.dataMap.find(feature1Name)->second.begin(), ts.dataMap.find(feature1Name)->second.end(), infoArr1);
        for (int j = i + 1; j < colNum; ++j) {
            string tempName = featuresNames[j];
            copy(ts.dataMap.find(tempName)->second.begin(), ts.dataMap.find(tempName)->second.end(), temp);
            // Checks correlation by the pearson method. If There is a correlation, saves it and turn on the flag.
            if (std::abs(pearson(infoArr1, temp, dataSize)) > correlation) {
                correlation = std::abs(pearson(infoArr1, temp, dataSize));
                feature2Name = tempName;
                flag = j;
            }
        }
        if (flag != -1) {
            // If There is a correlation, and it's bigger than 0.5 - create new 'correlatedFeatures' object.
            if (correlation > lowThreshHold) {
                copy(ts.dataMap.find(feature2Name)->second.begin(), ts.dataMap.find(feature2Name)->second.end(),
                     infoArr2);
                Point* pointsArr[dataSize];
                for (int k = 0; k < dataSize; ++k) {
                    pointsArr[k] = new Point(infoArr1[k], infoArr2[k]);
                }
                correlatedFeatures corrfea;
                corrfea.feature1 = feature1Name;
                corrfea.feature2 = feature2Name;
                corrfea.corrlation = correlation;
                // std::abs(pearson(infoArr1, infoArr2, dataSize));
                corrfea.lin_reg = linear_reg(pointsArr, dataSize);
                // If (0.5 <= correlation < 0.9) set the threshHold by HybridAnomalyDetector, otherwise continue.
                if (correlation < correlationThreshHold)
                    setThreshHold(corrfea, pointsArr, dataSize);
                else {
                    // Goes through all the points and set the threshHold by the farthest one
                    for (int j = 0; j < dataSize; ++j) {
                        if (dev(*pointsArr[j], corrfea.lin_reg) > corrfea.threshold) {
                            corrfea.threshold = dev(*pointsArr[j], corrfea.lin_reg);
                        }
                    }
                }
                cf.push_back(corrfea);
                // Deletes the 2D points array
                for (int j = 0; j < dataSize; ++j) {
                    delete pointsArr[j];
                }
            }
        }
    }
}

/*
 * Check anomalies from the new input accordingly the early learning.
 */
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    int dataSize = ts.getLineNum();
    for (int i = 0; i < cf.size(); ++i) {
        for (int j = 0; j < dataSize; ++j) {
            // Check anomaly. If there is a one - adds it to the anomalies vector.
            if (checkAnomaly(ts, cf[i], j)) {
                AnomalyReport anomalyReport = AnomalyReport(cf[i].feature1+"-"+cf[i].feature2, j+1);
                ar.push_back(anomalyReport);
            }
        }
    }
    return ar;
}
