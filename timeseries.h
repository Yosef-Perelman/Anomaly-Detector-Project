#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <utility>
#include <stdexcept>
#include <sstream>

using namespace std;

class TimeSeries{

    vector<string> feaName;
    int colNum = 0, lineNum;

public:

    map <string, vector<float>> dataMap;

    TimeSeries(const char* CSVfileName){

        //Open the CSV file
        ifstream infoFile(CSVfileName);

        //Check if the file opening succeed
        if (!infoFile) {
            cout << "The file isn't open" << std::endl;
        }

        // Helper variables
        string feature, row;
        float num;

        // Read the features name's row
        getline(infoFile, row);
        stringstream namesRow(row);
        while (getline(namesRow, feature,',')){
            colNum++;
            feaName.push_back(feature);
            dataMap.insert(pair<string, vector<float>>(feature, vector<float>{}));
        }

        //Read the data line by line
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
        lineNum = dataMap.begin()->second.size();
        infoFile.close();
    }

    //Get the row number
    const int getColNum() const{
        return colNum;
    }

    //Get the line number
    const int getLineNum() const{
        return lineNum;
    }

    /*//Get full information of feature
    const pair<string, vector<float>> getFeaInfo(string key) const{
        return pair<string, vector<float>>(dataMap.find(key)->first, dataMap.find(key)->second);
    }

    //Get values of feature
    const vector<float> getFeaValues(string key) const{
        return dataMap.find(key)->second;
    }*/

    //Get vector of strings with all the features name's with "," to separate
    const vector<string>& getFeaturesName() const{
        return feaName;
    }
};

#endif /* TIMESERIES_H_ */
