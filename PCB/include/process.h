//
// Created by raziyeh on 4/7/19.
//

#ifndef PCB_PROCESS_H
#define PCB_PROCESS_H

using namespace std;

enum Status {
    NEW = 0,
    RUNNING = 1,
    READY = 2
};

class Process{
public:
    Process();
    int getProcessId();
    int getPriority();
    Status getStatus();
    void setProcessId(int);
    void setPriority(int);
    void setStatus(Status);
private:
    int processId;
    int priority;
    Status status;
};


#endif //PCB_PROCESS_H
