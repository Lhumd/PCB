//
// Created by raziyeh on 4/7/19.
//
#include "../include/process.h"

Process::Process(int burst, int time, int id, int importance)
{
    process_id = id;
    burst_time = burst;
    burst_left = burst;
    arrival_time = time;
    been_seen = false;
    priority = importance;
    initial_wait = 0;
    total_wait_time = 0;
}

Process::Process()
{
    process_id = priority = 0;
    status = Status :: NEW;
}

int Process::getProcessId(){
    return process_id;
}

int Process::getPriority(){
    return priority;
}

Status Process::getStatus(){
    return status;
}

int Process::getArrival(){
    return arrival_time;
}

int Process::getBurst(){
    return burst_time;
}

int Process::getBurstLeft(){
    return burst_left;
}

int Process::getInitialWait(){
    return initial_wait;
}

int Process::getTotalWaitTime(){
    return total_wait_time;
}

void Process::addWait(int time){
    total_wait_time = total_wait_time + time;
}

bool Process::getSeen(){
    return been_seen;
}

void Process::setProcessId(int pid){
    process_id = pid;
}
void Process::setPriority(int pri){
    priority = pri;
}
void Process::setStatus(Status st){
    status = st;
}

void Process::setArrival(int time){
    arrival_time = time;
}

void Process::setPremp_time(int time){
    premp_time = time;
}

void Process::seen(int time){
    been_seen = true;
    initial_wait = time;
    total_wait_time = time;
}

void Process::decBurst(){
    burst_left = burst_left - 1;
}

