#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream

using namespace std;

class TimeSeries{

public:

    map <string, vector<float>> dataMap;

    TimeSeries(const char* CSVfileName){

        //Open the CSV file
        ifstream infoFile(CSVfileName);

        //Check if the file opening went well
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
        infoFile.close();
    }

    //Get the features name's
    string getFeaturesName(){
    }
};

#endif /* TIMESERIES_H_ */
