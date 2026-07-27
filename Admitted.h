#ifndef ADMITTED_H
#define ADMITTED_H

// ADMITTED Paitents - Three separate HashTable
// Check patient via Patient Name 
// Each ward has a fixed bed count (capacity)
// Hash by  patient ID; linear Probing for collision

// ICU : 10 beds ( Critical Patients with priority 1)
// OPD : 7 Beds  (Serious patients with priority 2)
// ExamWard : 2 Beds ( Priority 3 and 4 patients with abnormal test report)

#include<iostream>
#include<string>
using namespace std;

// AdmittedPatient - a lightweight record stored in each ward table
// Full record is in BST
 struct AdmittedPatient {
    int id;
    string name;
    int age;
    string disease;
    int priority;
    string status; // "Under Observation" or "Under Treatment"
    string doctorName; // which doctor is assigned
    bool occupied; // true = bed is occupied

    AdmittedPatient(){
        occupied = false;
        id = -1;
    }
 };

 // WardTable -> generic hashTable for any ward
#define SIZE 10 // max amount of beds = 10 

 struct WardTable{
    AdmittedPatient beds[SIZE];
    int capacity; // actual beds in the ward
    int count; // current patient admitted
    string WardName; // name of the ward

    WardTable(){
        capacity =0;
        count =0;
        WardName = "";
    }

    void init(string name, int cap){
        WardName = name;
        capacity = cap;
        count = 0;

        for(int i =0; i < SIZE ; i++){
            beds[i].occupied = false;
        }
    }
        int HashFunction(int id){
            return id % SIZE;

        }
        bool isFull(){
            return count >= capacity;
        }
        bool isEmpty(){
            return count == 0;
        }
        // Now admit a patient and return true if bed was found
        bool admitPatient(int id, string name, int age, string disease, int priority, string status, string doctorName){
            if(isFull()){
                cout<<"\n Beds are full in ward: "<< WardName;
                return false;
            }
            int idx = HashFunction(id);
                int c = 0; // count
                while(beds[idx].occupied && c < capacity){
                    idx = (idx + 1) % capacity;
                    c++;
                }
                if (c == capacity){
                    return false; // safety check
                }
        beds[idx].id         = id;
        beds[idx].name       = name;
        beds[idx].age        = age;
        beds[idx].disease    = disease;
        beds[idx].priority   = priority;
        beds[idx].status     = status;
        beds[idx].doctorName = doctorName;
        beds[idx].occupied   = true;
        count++;
            cout<<"\n ["<< WardName << "] Patient "<< name <<"(ID:" << id << ") has been admitted. Bed used: "<< count << "/"<< capacity << endl;
            return true;
        }

        // update the status of patient in the ward, when doctor has been assigned
        void updateStatus(int id, string newStatus, string newDoctor){
            int idx = HashFunction(id);
            int c =0; // count
            while(c < capacity){
                if(beds[idx].occupied && beds[idx].id == id){
                    beds[idx].status = newStatus;
                    beds[idx].doctorName = newDoctor;
                    return ;
                }
                idx = (idx +1)% capacity;
                c++;
            }
        }
        // Remove patient from ward (when discharged/ moved to billing)
        void removePatient(int id){
            int idx = HashFunction(id);
            int c =0; // count
            while(c < capacity){
                if(beds[idx].occupied && beds[idx].id== id){
                    beds[idx].occupied = false;
                    beds[idx].id = -1;
                    count --;
                    cout<<"\n [" <<WardName<< "] Patient " << id << " Discharged"<< endl;
                    return;
                }
                idx = (idx +1)% capacity;
                c++;
            }
        }
        // Display all occupied beds in the ward

        void Display(){
            if(isEmpty()){
                cout<< "\n " <<WardName<<" is empty";
                return;
            }
            cout<<"\n Ward Name: " << WardName << " (" << count << "/" << capacity << ")" <<endl;
            for(int i =0; i< capacity; i ++){
                if(!beds[i].occupied){
                    continue;
                }
                cout<< "Bed no. "<< i + 1 << ": "<<endl;
                cout<<"Patient ID: "<< beds[i].id << endl;
                cout<<"Patient Name: "<<beds[i].name<<endl;
                cout<<"Patient Age: "<<beds[i].age<<endl;
                cout<<"Patient Disease: "<< beds[i].disease << endl;
                cout<<"Priority: ";
                switch(beds[i].priority){
                    case 1:
                    cout<< "Critical\n";break;
                    case 2: 
                    cout<<" Serious\n";break;
                    case 3:
                    cout<<" Moderate\n";break;
                    case 4:
                    cout<<" Minor\n";break;
                }
                cout<<"Status: "<<beds[i].status<<endl;
                cout<<"Doctor: "<<(beds[i].doctorName.empty()? "Not yet assigned" : beds[i].doctorName)<<endl;
            }

        }
        //Find a patient in this ward by Name
        AdmittedPatient * findPatientbyName(string name){
            string target = name;
            for(int i =0; i<(int)target.size(); i++){
                target[i]=tolower(target[i]);
            }
            for(int i=0; i<capacity; i++){
                if(!beds[i].occupied){
                    continue;
                }
            
            string n = beds[i].name;
            for(int j =0; j<(int)n.size(); j++){
                n[j]= tolower(n[j]);
            }
            if(n == target){
                return &beds[i];
            }
        }
            return NULL ;
            
        }
        };
        // Admitted Wards _ hold all three wards
        struct AdmittedWards{
            WardTable ICU; // 10 beds - priority 1
            WardTable OPD; // 7 beds - priority 2
            WardTable ExamWard; // 2 beds - priority 3 or 4 with abnormal test report
        
            AdmittedWards(){
                ICU.init ("ICU", 10);
                OPD.init("OPD", 7);
                ExamWard.init("Examinationward", 2);
            }
            //Search a patient by name
            void searchbyName(string name){
                AdmittedPatient*found = NULL;
                string WardFound ="";

                found = ICU.findPatientbyName(name);
                if(found){
                    WardFound = "ICU";
                }
                if(!found){
                    found = OPD.findPatientbyName(name);
                    if(found){
                        WardFound = "OPD";
                    }
                }
                if(!found){
                    found = ExamWard.findPatientbyName(name);
                    if(found){
                        WardFound = "Examination Ward";
                    }
                }
                if(found){
                    cout<<"\n Patient found in Ward: "<< WardFound<<endl;
                    cout<<"Patient ID: "<<found->id<<endl;
                    cout<<"Patient Name: "<<found->name<<endl;
                    cout<<"Patient Age: "<<found->age<<endl;
                    cout<<"Disease: "<<found->disease<<endl;
                    cout<<"Ward: "<<WardFound<<endl;
                    cout<<"Status: " <<found->status<<endl;
                    cout<<"Doctor: "<<(found->doctorName.empty()? "Not assigned yet" : found->doctorName);
                }
                else {
                    cout<<"\nPatient: " << name <<" was not Found"<<endl;
                }
            }

            // Display a specific Ward

            void displayICU() {
                ICU.Display();
            }
            void displayOPD(){
                OPD.Display();
            }
            void displayExamWard(){
                ExamWard.Display();
            }

            // Display all wards at once
            void DisplayAll(){
                displayICU();
                displayOPD();
                displayExamWard();
            }
 };
 #endif
