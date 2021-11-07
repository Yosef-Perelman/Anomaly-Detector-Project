/*
 * anomaly_detection_util.cpp
 *
 * Author: Yosef Perelman 206344814 and Ariel Mantel 313450249
 */

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <fstream>
#include <map>
#include <vector>
#include <sstream>

using namespace std;

class TimeSeries{

    vector<string> FeaturesNames;
    int FeaturesNumber = 0, RowsNumber;

public:

    map <string, vector<float>> dataMap;

    TimeSeries(const char* CSVfileName){

        //Open the CSV file
        ifstream infoFile(CSVfileName);

        // Helper variables
        string feature, row;
        float num;

        // Get the names of the features
        getline(infoFile, row);
        stringstream namesRow(row);
        while (getline(namesRow, feature,',')){
            FeaturesNumber++;
            FeaturesNames.push_back(feature);
            // Insert to the map pairs like <1.A feature name, 2.Empty vector that will contain the feature data>
            dataMap.insert(pair<string, vector<float>>(feature, vector<float>{}));
        }

        // Insert the data to the appropriate vector in the map
        // Crawl over the file line by line and insert every number to the appropriate place
        map<string, vector<float>>::iterator itr = dataMap.begin();
        while (getline(infoFile, row)){
            stringstream infoRows(row);
            while (infoRows >> num) {
                itr->second.push_back(num);
                if (infoRows.peek() == ',') infoRows.ignore();
                itr++;
            }
            itr = dataMap.begin();
        }
        RowsNumber = dataMap.begin()->second.size();
        infoFile.close();
    }

    //Get the number of features
    const int getColNum() const{
        return FeaturesNumber;
    }

    //Get the rows number that represent the size of the data
    const int getLineNum() const{
        return RowsNumber;
    }

    //Get vector of strings with all the features name's with "," to separate
    const vector<string>& getFeaturesName() const{
        return FeaturesNames;
    }
};

#endif /* TIMESERIES_H_ */