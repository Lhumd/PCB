//
// Created by raziyeh on 4/7/19.
//
#include "process.h"

Process::Process()
{
    processId = priority = 0;
    status = Status :: NEW;
}
int Process::getProcessId(){
    return processId;
}
int Process::getPriority(){
    return priority;
}
Status Process::getStatus(){
    return status;
}

void Process::setProcessId(int pid){
    processId = pid;
}
void Process::setPriority(int pri){
    priority = pri;
}
void Process::setStatus(Status st){
    status = st;
}