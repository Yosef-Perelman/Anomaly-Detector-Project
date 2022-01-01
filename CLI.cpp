/*
 * anomaly_detection_util.cpp
 *
 * Author: Yosef Perelman 206344814 and Ariel Mantel 313450249
 */

#include "CLI.h"
#include <string>

CLI::CLI(DefaultIO* dio) {
    this -> dio = dio;
    allCommands.push_back(new UploadCSV(dio));
    allCommands.push_back(new Settings(dio));
    allCommands.push_back(new Detect(dio));
    allCommands.push_back(new Report(dio));
    allCommands.push_back(new UploadAnomalies(dio));
    allCommands.push_back(new Exit(dio));
}

void CLI::start(){
    information Information;
    while (true) {
        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");
        for (int i = 0; i < 6; i++) {
            dio->write((string(to_string(i + 1))) + ".");
            dio->write(allCommands[i]->description + "\n");
        }
        string input = dio->read();
        int choice = stoi(input) - 1; //because the vector starts from 0
        if (choice >= 0 && choice < 6) {
            if(5 == choice) {
                break;
            }
            allCommands[choice]->execute(Information);
        }
    }
}


CLI::~CLI() {
    for (int i = 0; i < 6; ++i) {
        delete(allCommands[i]);
    }
}

