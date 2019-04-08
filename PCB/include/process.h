//
// Created by raziyeh on 4/7/19.
//

#ifndef PCB_PROCESS_H
#define PCB_PROCESS_H

using namespace std;

enum Status {
    NEW = 0,
    RUNNING = 1,
    READY = 2,
    Terminated = 3,
    Waiting = 4
};

class Process{
public:
    Process();
    Process(int burst, int time, int id, int importance);
    int getProcessId();
    int getPriority();
    Status getStatus();
    int getArrival();
    int getBurst();;
    int getBurstLeft();
    int getInitialWait();
    int getTotalWaitTime();
    void addWait(int time);
    bool getSeen();

    void setProcessId(int);
    void setPriority(int);
    void setStatus(Status);
    void setArrival(int time);
    void setPremp_time(int time);
    void seen(int time);
    void decBurst();

private:
    int process_id;
    int priority;
    Status status;

    int arrival_time;       // Time that process arrives to cpu
    int burst_time;         // Time required to complete the process
    int premp_time;
    int burst_left;         // Time left for process to complete
    bool been_seen;         // Determines if process has been seen by cpu
    int initial_wait;       // Initail wait time for a process
    int total_wait_time;    // Total wait time a process endured.

};


#endif //PCB_PROCESS_H
