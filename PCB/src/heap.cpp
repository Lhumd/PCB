//
// Created by lhum on 4/7/19.
//

#include "../include/heap.h"

// Insert Process will add a new process into the queue
void Heap::insert(Process newProcess)
{
    newProcess.setStatus(Status :: READY);        // Updates status of the process
    heap.push_back(newProcess);         // Push the process to the rear of the queue
    heapifyup(heap.size() - 1);         // Heapify the new tree
}

// DeleteMin function that will delete the process with the highest priority.
int Heap::deletemin()
{
    int min = heap.front().getProcessId();   // Grab the existing minimum process
    heap.front().setStatus(Status :: READY);    // Update the status of the process being removed
    heap[0] = heap.at(heap.size() - 1);
    heap.pop_back();                    // Remove the highest priority from queue
    heapifydown(0);                     // Heapify the new tree
    return min;                         // Return the highest priority process id
}

// AgeQueue function will decrement the priority of each process in the queue
void Heap::ageQueue()
{
    for(int x = 0; x < heap.size(); x++){
        heap[x].setPriority(heap[x].getPriority()-1);
    }
}

// Print function will print the queue.
void Heap::print()
{
    cout << "ReadyQueue: ";
    for(int i = 0; i < heap.size(); i++)
    {
        cout << heap[i].getProcessId() << ":";
        cout << heap[i].getPriority() << ":";
        cout << heap[i].getStatus() << " ";
    }
    cout << endl;
}

// CheckExists function to check if the randomly generated process exists in the queue.
bool Heap::checkExists(int randProc)
{
    for(int x = 0; x < heap.size(); x++){
        if(randProc == heap[x].getProcessId())  // If the process exists in the queue
            return true;}                    // return true
    return false;                        // Otherwise, return false
}

// HeapifyUp function used when inserting
void Heap::heapifyup(int index)
{
    while((index>0)&&(parent(index)>=0)&&
          (heap[parent(index)].getPriority()>heap[index].getPriority()))
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
    if((l <= heapSize) && (heap[l].getPriority() <= heap[index].getPriority()))
        min = l;                              // Set mimumum to left child
    else
        min = index;                          // Set minimum to index
    if((r <= heapSize) && (heap[r].getPriority() <= heap[min].getPriority()))
        min = r;                              // Set minimum to right child
    if(min != index && min != -1)
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
