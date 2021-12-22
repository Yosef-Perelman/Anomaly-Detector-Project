

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
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
    void readFile(const string& path){
        std::ifstream infile(path);
        string line;
        string fileText;
        while (getline(infile, line)){
            if (line != "done"){
                fileText = fileText + line + "\n";
            } else{
                infile.close();
            }
        }
    }
};

// you may add here helper classes
struct information{
    float threshold;
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

    virtual void execute(information* information)=0;
    virtual ~Command(){}
};

// implement here your command classes
class UploadCSV:public Command{
public:
    UploadCSV(DefaultIO* dio):Command(dio,"upload a time series csv file"){}
    virtual void execute(information* information){
        for (int i = 0; i < 2; ++i) {
            dio->write("Please upload your local train CSV file.");
            string path = dio->read();

        }
    }
};

class Settings: public Command{
public:
    Settings(DefaultIO* dio): Command(dio, "algorithm settings"){}
    virtual void execute(information* information){
        bool shouldContinue = true;
        while (shouldContinue){
            dio->write("The current correlation threshold is ");
            dio->write(information->threshold);
            dio->write("\nType a new threshold\n");
            string input = dio->read();
            if (stof(input) > 0 && stof(input) < 1){
                shouldContinue = false;
                information->threshold = stof(input);
            } else {
                dio->write("please choose a value between 0 and 1");
            }
        }

    }
};

class Detect: public Command{
public:
    Detect(DefaultIO* dio): Command(dio, "detect anomalies"){}
    virtual void execute(information* information){

    }
};

class Report: public Command{
public:
    Report(DefaultIO* dio): Command(dio, "display results"){}
    virtual void execute(information* information){

    }
};

class UploadAnomalies: public Command{
public:
    UploadAnomalies(DefaultIO* dio): Command(dio, "upload anomalies and analyze results"){}
    virtual void execute(information* information){

    }
};

class Exit: public Command{
public:
    Exit(DefaultIO* dio): Command(dio, "exit"){}
    virtual void execute(information* information){

    }
};

#endif /* COMMANDS_H_ */
