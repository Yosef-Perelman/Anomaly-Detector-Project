#include <string>
#include <string.h>
#include <map>
#include <vector>
#include <algorithm>
#include "SimpleAnomalyDetector.h"

const float threshHold = 0.9;

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    // TODO Auto-generated constructor stub

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

//Get vector of floats and extract the info into array
void vecToArr(float* arr, vector<float> vec){

}

//Get two arrays and create 2D array of Points
/*Point** createPointsArray(float* arr1, float* arr2, int size){
    Point* pointsArray[size];
    for (int i = 0; i < size; ++i) {
        pointsArray[i] = new Point(arr1[i], arr2[i]);
    }
    return pointsArray;
}*/

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
    //Vector with the features names
    vector<string> featuresNames = ts.getFeaturesName();
    string feature1Name, feature2Name;
    int colNum = ts.getColNum(), dataSize = ts.getLineNum();
    // TODO initial the arrays dynamically
    float infoArr1[dataSize], infoArr2[dataSize], temp[dataSize];
    //Move on all the feature except the last
    for (int i = 0; i < colNum - 1; ++i) {
        int c = -1;
        float correlation = 0;
        feature1Name = featuresNames[i];
        copy(ts.dataMap.find(feature1Name)->second.begin(), ts.dataMap.find(feature1Name)->second.end(), infoArr1);
        for (int j = i + 1; j < colNum; ++j) {
            string tempName = featuresNames[j];
            copy(ts.dataMap.find(tempName)->second.begin(), ts.dataMap.find(tempName)->second.end(), temp);
            if (fabs(pearson(infoArr1, temp, dataSize)) > correlation) {
                correlation = fabs(pearson(infoArr1, temp, dataSize));
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
                corrfea.corrlation = fabs(pearson(infoArr1, infoArr2, dataSize));
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

    // TODO Auto-generated destructor stub
}

