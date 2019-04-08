//
// Created by lhum on 4/7/19.
//

#ifndef OS1_HEAP_H
#define OS1_HEAP_H

#include <iostream>
#include <vector>
#include "process.h"

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


#endif //OS1_HEAP_H
