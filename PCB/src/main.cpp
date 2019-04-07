#include <iterator>
#include "../include/heap.h"
#include <time.h>        // Used for Timer
#include <ctime>         // Used for randomization
#include <cstdlib>
#include "../include/process.h"

using namespace std;

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
        newProc.setProcessId(i);
        newProc.setPriority(i);
        newProc.setStatus("NEW");
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
            pcb_Table[randNum].setPriority(randPrior);   // Assign random priority to process
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
            pcb_Table[randNum].setPriority(randPrior);   // Assign random priority to process
            myHeap->insert(pcb_Table[randNum]);        // Insert non-existant process into queue.
        }

        // Determine total time needed to complete iterations.
        final = clock() - init;
        cout << "Total time: " << (double)final/((double)CLOCKS_PER_SEC) << " seconds." << endl;

        delete myHeap;
    }
}