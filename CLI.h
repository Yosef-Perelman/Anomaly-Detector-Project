/*
 * anomaly_detection_util.cpp
 *
 * Author: Yosef Perelman 206344814 and Ariel Mantel 313450249
 */

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
    vector<Command*> allCommands;
    DefaultIO* dio;
    // you can add data members
public:
    CLI(DefaultIO* dio);
    void start();
    virtual ~CLI();
};

#endif /* CLI_H_ */
