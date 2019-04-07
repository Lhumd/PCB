//==============================================================================
//                            PROCESS CONTROL BLOCK
// Names: Jeff Henry, Natalie Lotz
// File Type: .cpp
// How to compile: Type make in the unix command window. This will run the 
//                 makeFile document and construct the executable.
// How to run:     Type ./minHeap.exe in the unix command window. The User will 
//                 be prompted to run test case 1 or 2. Test case 1 contains a
//                 hardcoded example of processes entering the readyQueue. Test 
//                 case 2 processes 1,000,000 process's through the readyQueue 
//                 and outputs the total time taken to handle all processes.
//==============================================================================

#include <iostream>
#include <vector>
#include <iterator>
#include <time.h>        // Used for Timer
#include <ctime>         // Used for randomization
#include <cstdlib>

using namespace std;

// Process class to control all variables associated with an individual process
class Process{
public:
    // Individual Process variables
    int processId;
    int priority;
    string status;
    Process(){processId = priority = 0; status = "NEW";} // Initialize the process
};

// Heap Class to control all methods associated with the heap tree.
class Heap
{
public:
    Heap(){};                           // Default Constructor (Un-needed)
    ~Heap(){};                          // Secondary Constructor (Un-needed)
    void insert(Process newProcess);    // Inserts Process Object into queue.
    int deletemin();                    // Removes highest priority Process from queue.
    void ageQueue();                    // Ages all the processes in the queue.
    void print();                       // Prints the heap tree
    bool checkExists(int randProc);     // Checks whether the random process is in the queue already
    int size() { return heap.size(); }  // Returns the size of the heap tree
private:
    int left(int parent);               // Returns the left child
    int right(int parent);              // Returns the right child
    int parent(int child);              // Returns the parent process
    void heapifyup(int index);          // Heapifies the tree when inserted
    void heapifydown(int index);        // Heapifies the tree when removed
private:
    vector<Process> heap;               // ReadyQueue to place each process in.
};

// Insert Process will add a new process into the queue
void Heap::insert(Process newProcess)
{
    newProcess.status = "READY";        // Updates status of the process
    heap.push_back(newProcess);         // Push the process to the rear of the queue
    heapifyup(heap.size() - 1);         // Heapify the new tree
}

// DeleteMin function that will delete the process with the highest priority.
int Heap::deletemin()
{
    int min = heap.front().processId;   // Grab the existing minimum process
    heap.front().status = "RUNNING";    // Update the status of the process being removed
    heap[0] = heap.at(heap.size() - 1);
    heap.pop_back();                    // Remove the highest priority from queue
    heapifydown(0);                     // Heapify the new tree
    return min;                         // Return the highest priority process id
}

// AgeQueue function will decrement the priority of each process in the queue
void Heap::ageQueue()
{
    for(int x = 0; x < heap.size(); x++){
        heap[x].priority = heap[x].priority-1;}
}

// Print function will print the queue.
void Heap::print()
{
    cout << "ReadyQueue: ";
    for(int i = 0; i < heap.size(); i++)
    {
        cout << heap[i].processId << ":";
        cout << heap[i].priority << ":";
        cout << heap[i].status << " ";
    }
    cout << endl;
}

// CheckExists function to check if the randomly generated process exists in the queue.
bool Heap::checkExists(int randProc)
{
    for(int x = 0; x < heap.size(); x++){
        if(randProc == heap[x].processId)  // If the process exists in the queue
            return true;}                    // return true
    return false;                        // Otherwise, return false
}

// HeapifyUp function used when inserting
void Heap::heapifyup(int index)
{
    while((index>0)&&(parent(index)>=0)&&
          (heap[parent(index)].priority>heap[index].priority))
    {
        Process tmp = heap[parent(index)];  // Grab the parent process
        heap[parent(index)] = heap[index];  // Set parent process to index process
        heap[index] = tmp;                  // Set index process to parent process
        index = parent(index);              // Update index
    }
}

// HeapifyDown function used when deleting
void Heap::heapifydown(int index)
{
    int l,r,min;                            // Variables to hold priority values of children
    l = left(index);                        // Grab left child priority
    r = right(index);                       // Grab right child priority
    int heapSize = heap.size();             // Grab the size of the heap
    if((l <= heapSize) && (heap[l].priority <= heap[index].priority))
        min = l;                              // Set mimumum to left child
    else
        min = index;                          // Set minimum to index
    if((r <= heapSize) && (heap[r].priority <= heap[min].priority))
        min = r;                              // Set minimum to right child
    if(min != index)
    {
        Process t = heap[min];              // Create a process to hold the minimum process
        heap[min] = heap[index];            // Reassign minimum to index process
        heap[index] = t;                    // Reassign index process to minimum
        heapifydown(min);                   // Heapify the new tree.
    }
}

// Left function will return the left child of the parent 
int Heap::left(int parent)
{
    int i = ( parent << 1 ) + 1;            // Left Child is  2 * parent + 1
    return ( i < heap.size() ) ? i : -1;
}

// Right function will return the right child of the parent.
int Heap::right(int parent)
{
    int i = ( parent << 1 ) + 2;            // Right Child is 2 * parent + 2
    return ( i < heap.size() ) ? i : -1;
}

// Parent function will return the parent of the child
int Heap::parent(int child)
{
    if (child != 0){                        // If the child is not equal to 0
        int i = (child - 1) >> 1;             // Return the parent
        return i;
    }
    return -1;                              // Otherwise child doesn't exist
}

//======================================================================================
// Main Funtion
// This process will run the two tests for the Process Control Block
//======================================================================================
int main()
{
    int userChoice;
    cout << "Enter '1' for test 1 or '2' for test 2:";
    cin >> userChoice;
    // Process Table Array
    Process pcb_Table[20];
    // Initialize the first table:
    for(int i = 0; i < 20; i++)
    {
        Process newProc;
        newProc.processId = i;
        newProc.priority = i;
        newProc.status = "NEW";
        pcb_Table[i]= newProc;
    }
    // Create the heap tree
    Heap* myHeap = new Heap();

    //=========================================================================================
    //                                         TEST 1:
    // (a) Add Processes 5,1,8,and 11 to ReadyQueue. Display the contents of ReadyQueue
    // (b) Remove the process with teh highest priority from ReadyQueue and display ReadyQueue
    // (c) Remove the process with the highest priority from ReadyQueue and display ReadyQueue
    // (d) Insert processes 3.7,2,12, and 9 to ReadyQueue.
    // (e) Remove each process with highest priority from the ReadyQueue and display ReadyQueue
    //     after each removal.
    //=========================================================================================
    if(userChoice == 1)
    {
        myHeap->insert(pcb_Table[5]);
        myHeap->insert(pcb_Table[1]);
        myHeap->insert(pcb_Table[8]);
        myHeap->insert(pcb_Table[11]);
        myHeap->print();
        cout << "Deleted: " << myHeap->deletemin() << endl;
        myHeap->print();
        cout << "Deleted: " << myHeap->deletemin() << endl;
        myHeap->print();
        myHeap->insert(pcb_Table[3]);
        myHeap->insert(pcb_Table[7]);
        myHeap->insert(pcb_Table[2]);
        myHeap->insert(pcb_Table[12]);
        myHeap->insert(pcb_Table[9]);
        myHeap->print();
        while(myHeap->size() != 0)
            cout << "Deleted: " << myHeap->deletemin() << endl;

        // Cleanup
        delete myHeap;
    }
        //=========================================================================================
        //                                      TEST 2:
        // (a) Randomly select 10 processes from the pcb_Table.
        // (b) Add the processes to the readyQueue, assigning them each a random priority between 1
        //     and 50.
        //                            This is repeated 1,000,000 times.
        // (c) Remove one process
        // (d) Decrement priority values of processes still in the readyQueue.
        // (e) Randomly select one process from pcb_table that isn't already in the readyQueue.
        // (f) Add the process to the readyQueue, assigning it a random priority between 1 and 50.
        //
        //=========================================================================================
    else if(userChoice == 2)
    {
        int randNum,randPrior;                        // Random integers for randomization
        clock_t init, final;                          // Initial and final time objects
        init = clock();                               // Record the start time


        // Randomly select 10 processes from table:
        for(int x = 0; x < 10; x++)
        {
            randNum = rand()%19+1;                     // Generate a random number
            randPrior = rand()%50+1;                   // Randomize number between 1 and 50
            pcb_Table[randNum].priority = randPrior;   // Assign random priority to process
            myHeap->insert(pcb_Table[randNum]);        // Insert process into readyQueue
        }
        // 1,000,000 Iteration processes:
        for(int y = 0; y < 1000000; y++)
        {
            myHeap->deletemin();                       // Remove one process
            myHeap->ageQueue();                        // Age each process in the queue.
            do
            {
                randNum = rand()%19+1;                 // Generate a random number
            }
            while(myHeap->checkExists(randNum)!= false);
            randPrior = rand()%50+1;                   // Randomize number between 1 and 50
            pcb_Table[randNum].priority = randPrior;   // Assign random priority to process
            myHeap->insert(pcb_Table[randNum]);        // Insert non-existant process into queue.
        }

        // Determine total time needed to complete iterations.
        final = clock() - init;
        cout << "Total time: " << (double)final/((double)CLOCKS_PER_SEC) << " seconds." << endl;

        delete myHeap;
    }
}
