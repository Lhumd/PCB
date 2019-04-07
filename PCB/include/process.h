//
// Created by raziyeh on 4/7/19.
//

#ifndef PCB_PROCESS_H
#define PCB_PROCESS_H

#include <iostream>

using namespace std;

class Process{
public:
    Process();
    int getProcessId();
    int getPriority();
    string getStatus();
    void setProcessId(int);
    void setPriority(int);
    void setStatus(string);
private:
    int processId;
    int priority;
    string status;
};


#endif //PCB_PROCESS_H
