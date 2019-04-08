//
// Created by lhum on 4/8/19.
//

#include "../include/CPUScheduling.h"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <algorithm>
using namespace std;

//Initilizes variables related to statistical information
int context_switch = 14;
double total_turnaround_time = 0.0;
double total_wait_time = 0.0;
double total_initial = 0.0;
double max_turnaround_time = 0.0;
double min_turnaround_time = 5000.0;
double max_wait;
double min_wait;
double max_initial;
double min_initial;

//Creates Process class
class process {
private:
    int arrival_time;       // Time that process arrives to cpu
    int burst_time;         // Time required to complete the process
    int premp_time;
    int burst_left;         // Time left for process to complete
    bool been_seen;         // Determines if process has been seen by cpu
    int initial_wait;       // Initail wait time for a process
    int total_wait_time;    // Total wait time a process endured.
    int priority;           // Priority of process
    int pid;                // Process Id
    string status;          // Status of process
public:
    process(int burst, int time, int id, int importance);
    void set_status_ready();
    void set_status_waiting();
    void set_status_terminated();
    void set_status_running();
    void set_arrival(int time);
    void set_pid(int num);
    void set_premp_time(int time);
    void seen(int time);
    string get_status();
    int get_arrival();
    int get_burst();
    int get_priority();
    int get_burst_left();
    void dec_burst();
    int get_initial_wait();
    int get_total_wait_time();
    void add_wait(int time);
    bool get_seen();
    int get_pid();
};

process::process(int burst, int time, int id, int importance)
{
    pid = id;
    burst_time = burst;
    burst_left = burst;
    arrival_time = time;
    been_seen = false;
    priority = importance;
    initial_wait = 0;
    total_wait_time = 0;
}

// Status functions:
void process::set_status_ready(){status = "Ready";}
void process::set_status_running(){status = "Running";}
void process::set_status_terminated(){status = "Terminated";}
void process::set_status_waiting(){status = "Waiting";}
string process::get_status(){return status;}

// Set and Get functions:
void process::set_pid(int num){pid = num;}
bool process::get_seen(){return been_seen;}
int process::get_priority(){return priority;}
int process::get_burst(){return burst_time;}
int process::get_pid(){return pid;}
void process::set_arrival(int time){arrival_time = time;}
int process::get_arrival(){return arrival_time;}
int process::get_burst_left(){return burst_left;}
void process::dec_burst(){burst_left = burst_left - 1;}
int process::get_initial_wait(){return initial_wait;}
int process::get_total_wait_time(){return total_wait_time;}
void process::set_premp_time(int time){premp_time = time;}
void process::add_wait(int time){total_wait_time = total_wait_time + time;}
void process::seen(int time){been_seen = true;initial_wait = time;total_wait_time = time;}

// Sorting functions
bool burst_sorter(process a, process b){return a.get_burst() < b.get_burst();}
bool priority_sorter(process a, process b){return a.get_priority() < b.get_priority();}
bool arrival_sorter(process a, process b){return a.get_arrival() < b.get_arrival();}

// Print individual statistics for processes:
void individual_stats(process &curr_process)
{
    cout << "Process " << curr_process.get_pid() << endl;
    cout << "Burst Time: " << curr_process.get_burst() << " ms" << endl;
    cout << "Priority: " << curr_process.get_priority() << endl;
    cout << "Arrival Time: " << curr_process.get_arrival() << " ms" << endl;
    cout << "Initial Wait Time: " << curr_process.get_initial_wait() << " ms" << endl;
    cout << "Total Wait Time: " << curr_process.get_total_wait_time() << " ms" << endl;
    cout << "Status: " << curr_process.get_status() << endl;
    cout << "=============================================" << endl;
}

//Prints statistics
void statistics(int num_processes)
{
    cout << "Turnaround time:\t min ";
    printf("%.2f", min_turnaround_time);
    cout << " ms";
    cout << ";\t avg ";
    printf("%.2f", total_turnaround_time/num_processes);
    cout << " ms";
    cout << ";\t max ";
    printf("%.2f", max_turnaround_time);
    cout << " ms" << endl;
    cout << "Initial wait time:\t min ";
    printf("%.2f", min_initial);
    cout << " ms";
    cout << ";\t avg ";
    printf("%.2f", total_initial/num_processes);
    cout << " ms";
    cout << ";\t max ";
    printf("%.2f", max_initial);
    cout << " ms" << endl;
    cout << "Total wait time:\t min ";
    printf("%.2f", min_wait);
    cout << " ms";
    cout << ";\t avg ";
    printf("%.2f", total_wait_time/num_processes);
    cout << " ms";
    cout << ";\t max ";
    printf("%.2f", max_wait);
    cout << " ms" << endl;

    // Reinitialize the time values:
    total_turnaround_time = 0.0;
    total_wait_time = 0.0;
    total_initial = 0.0;
    max_wait = 0.0;
    min_wait = num_processes * 5000.0;
    max_initial = 0.0;
    min_initial = num_processes * 5000.0;
    max_turnaround_time = 0.0;
    min_turnaround_time = 5000.0 * num_processes;
    cout << endl;
}

//Simulates first come first serve algorithm:
void first_come_first_serve(vector <process> processes, int num_processes)
{
    cout << "First Come First Serve:" << endl;
    cout << "==========================================================================================" << endl;

    int num_completed = 0;      // Number of processes completed.
    int clock = 0;              // Clock counter
    int last_pid = 0;           // Most recent process
    vector <process> queue;     // Ready Queue
    bool cswitch = false;       // Context Switch Flag
    unsigned int index = processes.size();
    // Function runs until all processes has been seen and completed:
    while(num_completed != num_processes)
    {
        index = processes.size();
        for(unsigned int i = 0; i < index; i++)
        {
            // If a process is ready to arrive, add it to the ready queue.
            if (processes[i].get_arrival() <= clock)
            {
                processes[i].set_status_ready();
                queue.push_back(processes[i]);
                processes.erase(processes.begin() + i);
                index--;
                i--;
            }
        }
        if (queue.size() != 0)
        {
            // Context switch:
            if (cswitch)
            {
                clock = clock + 7;
                cswitch = false;
            }

            // Process entering CPU for first time:
            if (queue[0].get_seen() == false)
            {
                // Change the status of process to running:
                queue[0].set_status_running();
                //The initials will change in non zero arrival times
                int arriv = clock - queue[0].get_arrival();
                queue[0].seen(arriv);
                total_initial += arriv;
                if (arriv < min_initial)
                    min_initial = arriv;
                if (arriv > max_initial)
                    max_initial = arriv;
            }
            queue[0].dec_burst();

            // Process has been completed:
            if (queue[0].get_burst_left() == 0)
            {
                int fwait = ((clock + 1) - queue[0].get_burst() - queue[0].get_arrival());
                // Set status of process to be terminated
                queue[0].set_status_terminated();
                // Print the statistics of the individual process:
                individual_stats(queue[0]);
                last_pid = queue[0].get_pid();
                int turn = ((clock + 1) - queue[0].get_arrival());
                total_turnaround_time += turn;
                total_wait_time += fwait;
                if (fwait < min_wait)
                    min_wait = fwait;
                if (fwait > max_wait)
                    max_wait = fwait;
                if (turn < min_turnaround_time)
                    min_turnaround_time = turn;
                if (turn > max_turnaround_time)
                    max_turnaround_time = turn;
                queue.erase(queue.begin());
                num_completed++;

                if (queue.size() != 0)
                {
                    //cout << "[time " << clock + 1 << "ms] Context switch (swapped out process " << last_pid << " for process " << queue[0].get_pid() << ")" << endl;
                    clock = clock + context_switch;
                }
                else
                {
                    cswitch = true;
                    //Releases finished process
                    clock = clock + 7;
                }
            }
        }
        clock++;
    }
    // Print the overall Statistics:
    statistics(num_processes);
}

//Simulates SJF non-preemptive
void shortest_job_first(vector <process> processes, int num_processes)
{
    cout << "Shortest Job First:" << endl;
    cout << "==========================================================================================" << endl;

    int num_completed = 0;
    int clock = 0;
    int last_pid = 0;
    vector <process> queue;
    vector <process> temp;
    bool cswitch = false;
    unsigned int index = processes.size();
    while(num_completed != num_processes)
    {
        index = processes.size();
        for(unsigned int i = 0; i < index; i++)
        {
            // Process is ready to enter the queue:
            if (processes[i].get_arrival() <= clock)
            {
                // Set the status of process to be ready:
                processes[i].set_status_ready();
                // Add the process to the ready queue:
                temp.push_back(processes[i]);
                processes.erase(processes.begin());
                index--;
                i--;
            }
        }
        // Sort the processes by burst time:
        sort(temp.begin(), temp.end(), burst_sorter);
        for (unsigned int i = 0; i < temp.size(); i++)
        {
            queue.push_back(temp[i]);
        }
        temp.clear();

        //Sorts the processes by burst time length, ignores the currently running process because it is non-preemptive
        sort(queue.begin() + 1, queue.end(), burst_sorter);

        if (queue.size() != 0){
            // Context switch:
            if (cswitch)
            {
                clock = clock + 7;
                cswitch = false;
            }
            // Process has been seen by the cpu:
            if (queue[0].get_seen() == false)
            {
                //Statistical info
                int arriv = clock - queue[0].get_arrival();
                // Set status of process to be running:
                queue[0].set_status_running();
                queue[0].seen(arriv);
                total_initial += arriv;
                if (arriv < min_initial)
                    min_initial = arriv;
                if (arriv > max_initial)
                    max_initial = arriv;
            }
            // Decretment the process burst:
            queue[0].dec_burst();

            // Process has completed its burst time:
            if (queue[0].get_burst_left() == 0)
            {
                // Set status of process to terminated:
                queue[0].set_status_terminated();
                // Print the statistics of the process:
                individual_stats(queue[0]);
                int fwait = ((clock + 1) - queue[0].get_burst() - queue[0].get_arrival());
                last_pid = queue[0].get_pid();
                int turn = ((clock + 1) - queue[0].get_arrival());
                total_turnaround_time += turn;
                total_wait_time += fwait;
                if (fwait < min_wait)
                    min_wait = fwait;
                if (fwait > max_wait)
                    max_wait = fwait;
                if (turn < min_turnaround_time)
                    min_turnaround_time = turn;
                if (turn > max_turnaround_time)
                    max_turnaround_time = turn;
                queue.erase(queue.begin());
                // Another process completeted!
                num_completed++;

                if (queue.size() != 0)
                {
                    clock = clock + context_switch;
                }
                else
                {
                    cswitch = true;
                    //Releases finished process
                    clock = clock + 7;
                }
            }
        }
        clock++;
    }
    // Print the overall Statistics:
    statistics(num_processes);
}


//Simulates Shortest Job remaining (preemptive SJF)
void shortest_job_remaining(vector <process> processes, int num_processes)
{
    cout << "Shortest Job Remaining:" << endl;
    cout << "==========================================================================================" << endl;

    int num_completed = 0;
    int clock = 0;
    int last_pid = 0;
    vector <process> queue;
    bool cswitch = false;
    unsigned int index = processes.size();
    while(num_completed != num_processes)
    {
        index = processes.size();
        for(unsigned int i = 0; i < index; i++)
        {
            // Process is ready to be added to the queue:
            if (processes[i].get_arrival() <= clock)
            {
                // Context Switch on pre-emption
                if (queue.size() != 0 && processes[i].get_burst() < queue[0].get_burst() && queue[0].get_burst_left() != queue[0].get_burst())
                {
                    clock = clock + 14;
                }
                // Add the process to the queue:
                processes[i].set_status_ready();
                queue.push_back(processes[i]);
                sort(queue.begin(), queue.end(), burst_sorter);
                processes.erase(processes.begin());
                index--;
                i--;
            }
        }

        if (queue.size() != 0)
        {
            // Context Switch:
            if (cswitch)
            {
                clock = clock + 7;
                cswitch = false;
            }
            // Process is ready to be ran:
            if (queue[0].get_seen() == false)
            {
                queue[0].set_status_running();
                int arriv = clock - queue[0].get_arrival();
                queue[0].seen(arriv);
                total_initial += arriv;
                if (arriv < min_initial)
                    min_initial = arriv;
                if (arriv > max_initial)
                    max_initial = arriv;
            }
            // Decrement its burst time:
            queue[0].dec_burst();
            // Process has completed itself:
            if (queue[0].get_burst_left() == 0)
            {
                // Change the process status and print its statistics:
                queue[0].set_status_terminated();
                individual_stats(queue[0]);
                int fwait = ((clock + 1) - queue[0].get_burst() - queue[0].get_arrival());
                last_pid = queue[0].get_pid();
                int turn = ((clock + 1) - queue[0].get_arrival());
                total_turnaround_time += turn;
                total_wait_time += fwait;
                if (fwait < min_wait)
                    min_wait = fwait;
                if (fwait > max_wait)
                    max_wait = fwait;
                if (turn < min_turnaround_time)
                    min_turnaround_time = turn;
                if (turn > max_turnaround_time)
                    max_turnaround_time = turn;
                queue.erase(queue.begin());
                // Another Process completed!
                num_completed++;

                // Context Switch:
                if (queue.size() != 0)
                {
                    clock = clock + context_switch;
                }
                else
                {
                    cswitch = true;
                    //Releases finished process
                    clock = clock + 7;
                }
            }
        }
        clock++;
    }
    // Print the overall Statistics:
    statistics(num_processes);
}

//Simulates Round Robin Scheduling
void round_robin(vector <process> processes, int num_processes)
{
    cout << "Round Robin:" << endl;
    cout << "==========================================================================================" << endl;

    int num_completed = 0;
    int clock = 0;
    int last_pid = 0;

    //Sets the timeslice size
    int timeslice = 200;
    int current_slice = 0;
    vector <process> queue;
    unsigned int index = processes.size();
    bool cswitch = false;
    while(num_completed != num_processes)
    {
        for(unsigned int i = 0; i < index; i++)
        {
            // Process is ready to enter the queue:
            if (processes[i].get_arrival() <= clock)
            {
                processes[i].set_status_ready();
                queue.push_back(processes[i]);
                processes.erase(processes.begin());
                index--;
                i--;
            }
        }
        // Context Switch:
        if (queue.size() != 0)
        {
            if (cswitch)
            {
                clock = clock + 7;
                cswitch = false;
            }
            // Process is ready to run:
            if (queue[0].get_seen() == false)
            {
                queue[0].set_status_running();
                int initial = (clock - queue[0].get_arrival());
                queue[0].seen(initial);
                total_initial += initial;
                if (initial < min_initial)
                    min_initial = initial;
                if (initial > max_initial)
                    max_initial = initial;
            }
            // Decrement its burst time:
            queue[0].dec_burst();
            // Increment the time slice:
            current_slice++;
            // Process has completed itself
            if (queue[0].get_burst_left() == 0)
            {
                // Change status of process and print its statistics:
                queue[0].set_status_terminated();
                individual_stats(queue[0]);
                int total_wait_time_p = ((clock + 1) - queue[0].get_arrival() - queue[0].get_burst());
                last_pid = queue[0].get_pid();
                int turnaround = ((clock + 1) - queue[0].get_arrival());
                total_turnaround_time += turnaround;
                total_wait_time += total_wait_time_p;
                if (total_wait_time_p < min_wait)
                    min_wait = total_wait_time_p;
                if (total_wait_time_p > max_wait)
                    max_wait = total_wait_time_p;
                if ((turnaround) < min_turnaround_time)
                    min_turnaround_time = turnaround;
                if ((turnaround) > max_turnaround_time)
                    max_turnaround_time = turnaround;
                queue.erase(queue.begin());
                // Another Process Completed!
                num_completed++;

                // Context Switch:
                if (queue.size() != 0)
                {
                    clock = clock + context_switch;
                }
                else
                {
                    cswitch = true;
                    //Releases finished process
                    clock = clock + 7;
                }
                current_slice = 0;
            }
                // Time quantum is over.
            else if (current_slice == timeslice) //When timeslice is over
            {
                last_pid = queue[0].get_pid();
                queue.push_back(queue[0]);
                queue.erase(queue.begin());
                current_slice = 0;
                clock = clock + context_switch;
            }
        }
        clock++;
    }
    // Print the overall Statistics:
    statistics(num_processes);
}


//Simulates priority scheduling
void priority(vector <process> processes, int num_processes)
{
    cout << "Priority:" << endl;
    cout << "==========================================================================================" << endl;

    int num_completed = 0;
    int clock = 0;
    int last_pid = 0;

    //Sets the timeslice size
    int timeslice = 200;
    int current_slice = 0;
    vector <process> queue;
    vector <process> temp_queue;
    unsigned int index = processes.size();
    unsigned int index2;
    bool cswitch = false;
    int most_priority = 6;
    while(num_completed != num_processes)
    {
        for(unsigned int i = 0; i < index; i++)
        {
            // Process is ready to enter the CPU
            if (processes[i].get_arrival() <= clock)
            {
                processes[i].set_status_ready();
                temp_queue.push_back(processes[i]);
                index2 = temp_queue.size();
                // Sort processes by priority:
                sort(temp_queue.begin(), temp_queue.end(), priority_sorter);
                //Finds highest priority present
                if(processes[i].get_priority() < most_priority)
                    most_priority = processes[i].get_priority();
                processes.erase(processes.begin());
                index--;
                i--;
            }
        }
        index2 = temp_queue.size();
        for(unsigned int i = 0; i < index2; i++)
        {
            // Check each process for priority:
            if (temp_queue[i].get_priority() == most_priority)
            {
                // If the processes' priority is less than that current queue's process, context switch.
                if (queue.size() != 0 && temp_queue[i].get_priority() < queue[0].get_priority())
                {
                    clock = clock + 14;
                    for (unsigned int x = 0; x < queue.size(); x++)
                    {
                        temp_queue.push_back(queue[x]);
                    }
                    queue.clear();
                }
                queue.push_back(temp_queue[i]);
                temp_queue.erase(temp_queue.begin());
                index2--;
                i--;
            }
        }

        //Uses round robin for multiple processes with same highest priority
        if (queue.size() != 0 && queue.size() > 1)
        {
            // Context Switch:
            if (cswitch)
            {
                clock = clock + 7;
                cswitch = false;
            }
            // Process is ready to run:
            if (queue[0].get_seen() == false)
            {
                queue[0].set_status_running();
                //The initials will change in non zero arrival times
                int initial = (clock - queue[0].get_arrival());
                queue[0].seen(initial);
                total_initial += initial;
                if (initial < min_initial)
                    min_initial = initial;
                if (initial > max_initial)
                    max_initial = initial;
                //total_wait_time += initial;
            }
            // Decrement its cpu burst:
            queue[0].dec_burst();
            // Increase the time slice:
            current_slice++;
            // Process is completed!
            if (queue[0].get_burst_left() == 0)
            {
                // Change the process state and print out its statistics:
                queue[0].set_status_terminated();
                individual_stats(queue[0]);
                int total_wait_time_p = ((clock + 1) - queue[0].get_arrival() - queue[0].get_burst());
                last_pid = queue[0].get_pid();
                int turnaround = ((clock + 1) - queue[0].get_arrival());
                total_turnaround_time += turnaround;
                total_wait_time += total_wait_time_p;
                if (total_wait_time_p < min_wait)
                    min_wait = total_wait_time_p;
                if (total_wait_time_p > max_wait)
                    max_wait = total_wait_time_p;
                if ((turnaround) < min_turnaround_time)
                    min_turnaround_time = turnaround;
                if ((turnaround) > max_turnaround_time)
                    max_turnaround_time = turnaround;
                queue.erase(queue.begin());
                // Another process completed:
                num_completed++;
                if (queue.size() != 0)
                {
                    clock = clock + context_switch;
                }
                else
                {
                    cswitch = true;
                    //Releases finished process
                    clock = clock + 7;
                    if (temp_queue.size() != 0)
                        most_priority = temp_queue[0].get_priority();
                    else
                        most_priority++;
                }
                current_slice = 0;
            }
                // Process ran out of its time quantum:
            else if (current_slice == timeslice)
            {
                last_pid = queue[0].get_pid();
                queue.push_back(queue[0]);
                queue.erase(queue.begin());
                current_slice = 0;
                clock = clock + context_switch;

            }
        }
            // If there is only one process yet, no need to stop it from running at any point:
        else if (queue.size() == 1)
        {
            if (cswitch)
            {
                clock = clock + 7;
                cswitch = false;
            }
            // Process is ready to be running:
            if (queue[0].get_seen() == false)
            {
                queue[0].set_status_running();
                //The initials will change in non zero arrival times
                int arriv = clock - queue[0].get_arrival();
                queue[0].seen(arriv);
                total_initial += arriv;
                if (arriv < min_initial)
                    min_initial = arriv;
                if (arriv > max_initial)
                    max_initial = arriv;
                //total_wait_time += clock;
            }
            // Decrement its burst:
            queue[0].dec_burst();
            // Process is completed:
            if (queue[0].get_burst_left() == 0)
            {
                // Change status of process and print its statistics:
                queue[0].set_status_terminated();
                individual_stats(queue[0]);
                int fwait = ((clock + 1) - queue[0].get_burst() - queue[0].get_arrival());
                last_pid = queue[0].get_pid();
                int turn = ((clock + 1) - queue[0].get_arrival());
                total_turnaround_time += turn;
                total_wait_time += fwait;
                if (fwait < min_wait)
                    min_wait = fwait;
                if (fwait > max_wait)
                    max_wait = fwait;
                if (turn < min_turnaround_time)
                    min_turnaround_time = turn;
                if (turn > max_turnaround_time)
                    max_turnaround_time = turn;
                queue.erase(queue.begin());
                // Another process completed:
                num_completed++;
                if (queue.size() != 0)
                {
                    clock = clock + context_switch;
                }
                else
                {
                    cswitch = true;
                    //Releases finished process
                    clock = clock + 7;
                    if (temp_queue.size() != 0)
                        most_priority = temp_queue[0].get_priority();
                    else
                        most_priority++;
                }
            }
        }
        else if (queue.size() == 0)
        {
            if (temp_queue.size() != 0)
                most_priority = temp_queue[0].get_priority();
            else
                most_priority++;
        }
        clock++;
    }
    // Print the overall Statistics:
    statistics(num_processes);
}

void print_menu()
{
    cout << "=============================================================================================" << endl;
    cout << "                                   CPU SCHDULER EMULATOR" << endl;
    cout << "    Enter a number from the list below to test an algorithm [Base # Processes: 10]" << endl;
    cout << "    Algorithms:      1. FCFS" << endl;
    cout << "                     2. SJF" << endl;
    cout << "                     3. SRJF" << endl;
    cout << "                     4. RR"   << endl;
    cout << "                     5. Priority" << endl;
    cout << "                     6. Run all" << endl;
    cout << "                     7. Run FCFS with 20 processes" << endl;
    cout << "                     8. Run FCFS with 100 processes" << endl;
    cout << "=============================================================================================" << endl;
}

void generate_arrival(vector <process> &processes, int num_processes)
{
    srand((unsigned)time(0));
    int random_burst;
    int importance;
    //80% of processes have a semi random arrival time determined by exponential distribution
    //20% of processes have 0 arrival time
    int num_rand = num_processes * .8;
    int num_zero = num_processes - num_rand;

    //Creates processes with zero arrival time
    for (int i = 0; i < num_zero; i++)
    {
        random_burst = (rand()%3500)+500;
        importance = (rand()%5);
        process temp(random_burst, 0, i+1, importance);
        processes.push_back(temp);
    }

    //Creates processes with non zero arrival time
    for(int i = 0; i < num_rand; i++)
    {
        random_burst = (rand()%3500)+500;
        importance = (rand()%5);
        double lambda = 0.001;
        double r = ((double) rand()/(RAND_MAX));
        double x = -(log(r)/lambda);
        if ( x > 8000 ) { i--; continue; }
        process temp(random_burst, (int)x, (num_zero + i+1), importance);
        processes.push_back(temp);
    }
}


int main()
{
    int userChoice = 0;

    //Sets number of processes
    int num_processes = 10;

    srand((unsigned)time(0));
    int random_burst;
    int importance;

    // vector will serve as the ready queue
    vector <process> processes;

    // Generate the arrival times for all the processes:
    generate_arrival(processes, num_processes);

    // Sorts processes by arrival time
    sort(processes.begin(), processes.end(), arrival_sorter);

    // Assigns pids 1 - n based on arrival time. Makes the output easier to follow
    for (unsigned int i = 0; i < processes.size(); i++){processes[i].set_pid(i+1);}

    // Sets variables for statistical information
    max_wait = 0.0;
    min_wait = num_processes * 5000.0;
    max_initial = 0.0;
    min_initial = num_processes * 5000.0;

    // Print the menu and run what the user wants:
    print_menu();
    cin >> userChoice;
    if(userChoice == 1){first_come_first_serve(processes, num_processes);}
    else if(userChoice == 2){shortest_job_first(processes, num_processes);}
    else if(userChoice == 3){shortest_job_remaining(processes, num_processes);}
    else if(userChoice == 4){round_robin(processes, num_processes);}
    else if(userChoice == 5){priority(processes, num_processes);}
    else if(userChoice == 6){
        first_come_first_serve(processes, num_processes);
        shortest_job_first(processes, num_processes);
        shortest_job_remaining(processes, num_processes);
        round_robin(processes, num_processes);
        priority(processes, num_processes);
    }
    else if(userChoice == 7){
        num_processes = 20;
        generate_arrival(processes,num_processes);
        sort(processes.begin(), processes.end(), arrival_sorter);
        for(unsigned int i = 0; i < processes.size(); i++){processes[i].set_pid(i+1);}
        first_come_first_serve(processes, num_processes);
    }
    else if(userChoice == 8){
        num_processes = 100;
        generate_arrival(processes,num_processes);
        sort(processes.begin(), processes.end(), arrival_sorter);
        for(unsigned int i = 0; i < processes.size(); i++){processes[i].set_pid(i+1);}
        first_come_first_serve(processes, num_processes);
    }
    else return(0);

    return 0;
}