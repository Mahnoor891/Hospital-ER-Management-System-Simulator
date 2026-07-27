#ifndef ERWAITINGROOM_H
#define ERWAITINGROOM_H
// Patients will called by priority level (USING MIN HEAP)
// priority 3 and 4 patients wait here
// We will place the the critical patient (i.e lowest number at top)
//--------------------------------------------------------------------
#include<iostream>
using namespace std;
#include"Paitent.h"

struct Priority {
    Patient heap[100];
    int size =0;
   


void UpHeapify(int idx){

    while(idx>0){
        int parent = (idx - 1)/2;
        if(idx > 0 && heap[idx].priority < heap[parent].priority){
            swap(heap[idx], heap[parent]);
            idx = parent;
        }
        else {
            break;
        }
    }
}
void DownHeapify(int idx){
    // By this function, we put the least priority paitent at down and urgent one at the top
    while(true){
        int left = 2 *idx +1; //Left child
        int right = 2*idx +2; //right child
        int smallest =idx;
        if (left < size && heap[left].priority < heap[smallest].priority){
            smallest = left;
        }
        if(right < size && heap[right].priority < heap[smallest].priority){
            smallest = right;
        }

        if(smallest != idx){
            swap(heap[idx], heap[smallest]);
            idx = smallest;
           
        }
        else {
            break;
        }
    }
}
    void Enqueue(Patient p){
        if(size >=100){
            cout<<"Queue is full";
            return;
        }
        heap[size] = p;
        UpHeapify(size);
        size++;
    }

Patient Dequeue(){
    Patient top = heap[0];
    heap [0] = heap[size-1];
    size--;
    DownHeapify(0);
    return top;
}
bool isEmpty() {
    return size ==0;
}

void ERdisplay(){
    if(isEmpty()){
        cout<<"No paitents waiting";
        return;
    }
    cout <<"Waiting Patients: \n";
    for(int i =0; i < size; i ++){
        cout<< i + 1 << ": Paitent ID: "<< heap[i].id << endl;
        cout<<"Name: "<< heap[i].name<<endl;
        cout<<"Priority: "<< heap[i].priority<<endl;
        cout<<"Disease: "<< heap[i].disease << endl;
        cout<<"Location: "<<heap[i].location<<endl;
        cout<<"Condition: ";
        switch(heap[i].priority){
            case 3:
            cout<<"Moderate";
            break;
            case 4:
            cout<<"Minor";
            break;
        }
        cout<<" | Status: " << heap[i].status << "\n";
    }
}
};

#endif

