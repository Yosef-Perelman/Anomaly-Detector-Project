/*
 * anomaly_detection_util.cpp
 *
 * Author: Yosef Perelman 206344814 and Ariel Mantel 313450249
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
//#include <iomanip>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
    virtual string read()=0;
    virtual void write(string text)=0;
    virtual void write(float f)=0;
    virtual void read(float* f)=0;
    virtual ~DefaultIO(){}

    // you may add additional methods here
    /*void readFile(const string& path){
        std::ifstream infile(path);
        string line;
        string fileText;
        while (getline(infile, line)){
            if (line != "done"){
                fileText += line + "\n";
            } else{
                infile.close();
            }
        }
    }*/
};

// you may add here helper classes
struct information{
    float threshold;
    vector<AnomalyReport> ar;
    int nonAnomalies;
    vector<Point> allReports;
    information(){
        threshold = 0.9;
    }
};

// you may edit this class
class Command{
protected:
    DefaultIO* dio;
public:
    const string description;
    Command(DefaultIO* dio, const string description):dio(dio), description(description){}

    virtual void execute(information &Information)=0;
    virtual ~Command(){}
};

// implement here your command classes
class UploadCSV:public Command{

public:
    UploadCSV(DefaultIO* dio):Command(dio,"upload a time series csv file"){}

    void execute(information &Information) override{ // removed the 'virtual' and added 'override'
        dio->write("Please upload your local train CSV file.\n");
        ofstream anomalyTrain("anomalyTrain.csv");
        string line = dio->read();
        while(line != "done") {
            anomalyTrain << line << "\n";
            line = dio->read();
        }
        dio->write("Upload complete.\n");
        dio->write("Please upload your local test CSV file.\n");
        ofstream anomalyTest("anomalyTest.csv");
        line = dio->read();
        while(line != "done") {
            anomalyTest << line << "\n";
            line = dio->read();
        }
        dio->write("Upload complete.\n");
    }
};

class Settings: public Command{

public:
    Settings(DefaultIO* dio): Command(dio, "algorithm settings"){}

    void execute(information &Information) override{
        while (true){
            dio->write("The current correlation threshold is ");
            dio->write(Information.threshold);
            dio->write("\n");
            dio->write("Type a new threshold\n");
            string input = dio->read();
            float th = stof(input);
            if (th >= 0 && th <= 1){ // added '='
                Information.threshold = th;
                break;
            }
            else {
                dio->write("please choose a value between 0 and 1");
            }
        }
    }
};

class Detect: public Command{
public:

    Detect(DefaultIO* dio): Command(dio, "detect anomalies"){}

    void execute(information &Information) override{
        TimeSeries train("anomalyTrain.csv");
        HybridAnomalyDetector ad;
        ad.setCorrelationTheresHold(Information.threshold);
        ad.learnNormal(train);
        TimeSeries test("anomalyTest.csv");
        Information.ar = ad.detect(test);
        Information.nonAnomalies = test.getLineNum() - Information.ar.size();
        if (!Information.ar.empty()) {
            string description = Information.ar[0].description;
            float a = float(Information.ar[0].timeStep);
            for (int i = 1; i < Information.ar.size(); ++i) {
                if (Information.ar[i].description != description){
                    description = Information.ar[i].description;
                    Information.allReports.push_back({a, float(Information.ar[i - 1].timeStep)});
                    a = float(Information.ar[i].timeStep);
                }
            }
            Information.allReports.push_back({a, float(Information.ar.back().timeStep)});
        }
        dio->write("anomaly detection complete.\n");
    }
};


class Report: public Command{
public:
    Report(DefaultIO* dio): Command(dio, "display results"){}
    void execute(information &Information) override{
        size_t i = Information.ar.size();
        for (size_t j = 0; j < i; ++j) {
            string timeStep = to_string(Information.ar[j].timeStep);
            dio->write(timeStep + "\t" + Information.ar[j].description + "\n");
        }
        dio->write("Done.\n");
    }
};

class UploadAnomalies: public Command{
public:
    UploadAnomalies(DefaultIO* dio): Command(dio, "upload anomalies and analyze results"){}

    Point splitReport(const string str){
        string word;
        float a, b;
        int start = 0;
        int end = str.find(',');
        a = stof(str.substr(start, end));
        b = stof(str.substr(end + 1));
        return Point(a, b);
    }

    void execute(information &Information) override{
        dio->write("Please upload your local anomalies file.\n");
        vector<Point> reports;
        string report = dio->read();
        while (report != "done"){
            reports.push_back(splitReport(report));
            report = dio->read();
        }
        dio->write("Upload complete.\n");
        float truePositive = 0, falsePositive = 0;
        vector<Point>::iterator ptr;
        for (ptr = reports.begin(); ptr < reports.end(); ptr++){
            vector<Point>::iterator iter;
            for (iter = Information.allReports.begin(); iter < Information.allReports.end(); iter++){
                if (ptr->y >= iter->x && iter->y >= ptr->x){
                    ++truePositive;
                    break;
                }
            }
        }
        int flag = 0;
        for (ptr = Information.allReports.begin(); ptr < Information.allReports.end(); ptr++){
            flag = 0;
            vector<Point>::iterator iter;
            for (iter = reports.begin(); iter < reports.end(); iter++){
                if (ptr->y >= iter->x && iter->y >= ptr->x)
                    ++flag;
            }
            if (flag == 0)
                ++falsePositive;
        }
        float x = truePositive / float(reports.size());
        float y = falsePositive / float(Information.nonAnomalies);
        float tpr= ((int) (x * 1000.0)) / 1000.0f;
        float fpr= ((int) (y * 1000.0)) / 1000.0f;
        dio->write("True Positive Rate: ");
        dio->write(tpr);
        dio->write("\nFalse Positive Rate: ");
        dio->write(fpr);
        dio->write("\n");
    }
};

class Exit: public Command{
public:
    Exit(DefaultIO* dio): Command(dio, "exit"){}
    void execute(information &Information) override{
    }
};

#endif /* COMMANDS_H_ */
