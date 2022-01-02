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

// struct that hold some members for the implementation of the commands
struct information{
    float threshold; // The minimum number to consider "correlation" between two features
    vector<AnomalyReport> ar; // vector of anomalies
    int nonAnomalies; // the number of data lines from the test data that don't made anomaly
    vector<Point> allReports; // vector of deviation. its look like: '<(73, 76), (102, 105)...>

    information(){
        threshold = 0.9;
    }
};

// Abstract class of command
class Command{
protected:
    DefaultIO* dio;
public:
    const string description;
    Command(DefaultIO* dio, const string description):dio(dio), description(description){}

    virtual void execute(information &Information)=0;
    virtual ~Command(){}
};


class UploadCSV:public Command{

public:
    UploadCSV(DefaultIO* dio):Command(dio,"upload a time series csv file"){}

    // Read the data and create two files from it: "anomalyTrain" and "anomalyTest"
    void execute(information &Information) override{
        dio->write("Please upload your local train CSV file.\n");
        ofstream anomalyTrain("anomalyTrain.csv");
        // Read the data and write it into "anomalyTrain" line by line
        string line = dio->read();
        while(line != "done") {
            anomalyTrain << line << "\n";
            line = dio->read();
        }
        dio->write("Upload complete.\n");

        dio->write("Please upload your local test CSV file.\n");
        // Read the data and write it into "anomalyTest" line by line
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

    // Get new correlation threshold between 0 and 1 from the user
    void execute(information &Information) override{
        while (true){
            dio->write("The current correlation threshold is ");
            dio->write(Information.threshold);
            dio->write("\n");
            dio->write("Type a new threshold\n");
            string input = dio->read();
            float th = stof(input);
            if (th > 0 && th <= 1){
                Information.threshold = th;
                break;
            }
            else {
                dio->write("please choose a value between 0 and 1.\n");
            }
        }
    }
};

class Detect: public Command{
public:

    Detect(DefaultIO* dio): Command(dio, "detect anomalies"){}

    // Learn the data from the train and detect anomalies from the data from the test
    void execute(information &Information) override{
        Information.allReports.clear();
        TimeSeries train("anomalyTrain.csv");
        HybridAnomalyDetector ad;
        // In the anomaly detector detect anomalies according the correlation threshold that the user choose
        ad.setCorrelationTheresHold(Information.threshold);
        ad.learnNormal(train);
        TimeSeries test("anomalyTest.csv");
        Information.ar = ad.detect(test);
        // Save the number of lines un the data that don't cause anomaly
        Information.nonAnomalies = test.getLineNum() - Information.ar.size();
        if (!Information.ar.empty()) {
            //string description = Information.ar[0].description;
            // Create vector of deviations. its look like: '<(73, 76), (102, 105)...>
            // It move over the anomalies vector and check continuity of the report
            // by the timeSteps. If two reports aren't consecutive saves the deviation
            // and start to check the new deviation.
            float a = float(Information.ar[0].timeStep);
            for (int i = 1; i < Information.ar.size(); ++i) {
                if (Information.ar[i].timeStep != (Information.ar[i - 1].timeStep + 1)){
                    //description = Information.ar[i].description;
                    Information.allReports.push_back({a, float(Information.ar[i - 1].timeStep)});
                    a = float(Information.ar[i].timeStep);
                }
            }
            // In the end save the last deviation because in the loop it save deviations only when
            // a new deviation starts, and after the last deviation no deviation will start.
            Information.allReports.push_back({a, float(Information.ar.back().timeStep)});
        }
        dio->write("anomaly detection complete.\n");
    }
};


class Report: public Command{
public:
    Report(DefaultIO* dio): Command(dio, "display results"){}

    // Print all the anomalies like: "timeStep-timeStep  description"
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

    // Helper method that get string "timeStep, timeStep" and save it as Point which means two floats.
    Point splitReport(const string str){
        string word;
        float a, b;
        int start = 0;
        int end = str.find(',');
        a = stof(str.substr(start, end));
        b = stof(str.substr(end + 1));
        return Point(a, b);
    }

    // Compare anomalies report file and the data from the detection earlier
    void execute(information &Information) override{
        dio->write("Please upload your local anomalies file.\n");
        vector<Point> reports; // Vector of all the reports from the file
        string report = dio->read();
        while (report != "done"){
            reports.push_back(splitReport(report));
            report = dio->read();
        }
        dio->write("Upload complete.\n");
        // truePositive it's all the reports from the file that feet's (even partly) deviation from the detection.
        // falsePositive it's all the deviations from the detection that didn't feet any of the reports from the file.
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
        // To display the results with max of three digits after the decimal point,
        // first multiply it by 1000 to move the point three jumps left and convert it to int,
        // and then divided it by 1000.0 and convert it back to float
        int a = x * 1000, b = y * 1000;
        dio->write("True Positive Rate: ");
        dio->write(a / 1000.0);
        dio->write("\nFalse Positive Rate: ");
        dio->write(b / 1000.0);
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
