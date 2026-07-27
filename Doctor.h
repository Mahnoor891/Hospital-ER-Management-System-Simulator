#ifndef DOCTOR_H
#define DOCTOR_H

#include<iostream>
#include<string>

using namespace std;
// Storing Doctor's Record: 
#define SIZE 10

struct Doctor {
    string department;
    string name;
    int room;
    bool occupied; // true = this doctor has been registered
    bool available; // true = doctor is free to take a patient
    int treatingPatientID; //which patient is being treated
    int treatingPriority; // severity of paitent's condition being treated
    string treatingPatientName; //name of the paitent being treated
    string treatingDisease;
    bool bloodTestPending;
    // blood test is for priority 3 patients
    // waiting for the user to enter the result
    // for positive, the patient will be admitted to the ward
    // For negative,it will be set free after 10 minutes of checkup


    Doctor() {
        occupied =false;
        available = true;
        treatingPatientID = -1;
        treatingPatientName = "";
        treatingDisease = "";
        treatingPriority = -1;
        bloodTestPending = false;
    }
};
struct Patient;
struct Priority;
struct BSTNode;
struct BST;
struct BillingQueue;
struct AdmittedWards;
//Hash Table of Doctors
struct HashTable{
    Doctor table[SIZE];
       string toLowerCase(string s) {
            for(int i =0; i<(int)s.length(); i++){
                s[i] = tolower(s[i]);
            }
            return s;
        }
    int HashFunction(string key){
        int sum =0;
        for(int i=0; i<(int)key.length(); i++){
            sum +=key[i];
        }
            return sum % SIZE;
    }
    // map the disease to the concern department
    // Check if a disease string contains any keyword that belonged to a department
    // Return the matched department name, or " "if no match found.
    string matchDiseaseToDept(string disease){
        string d = toLowerCase(disease);

        // Cardiology keywords
        if(d.find("heart")!= string::npos ||
        d.find("cardiac")!= string::npos||
        d.find("chest")!= string::npos||
        d.find("coronary")!= string::npos||
        d.find("artery")!= string::npos ||
        d.find("angina")!= string::npos||
        d.find("attack")!= string::npos)
return "Cardiology";

//Neurology keywords
if(d.find("brain")!= string::npos||
        d.find("neuro")    != string::npos ||
        d.find("stroke")   != string::npos ||
        d.find("seizure")  != string::npos ||
        d.find("epilepsy") != string::npos ||
        d.find("migraine") != string::npos ||
        d.find("nerve")    != string::npos ||
        d.find("tumor")    != string::npos)
return "Neurology";

//Orthopedic keywords
if(d.find("bone") != string::npos ||
        d.find("fracture") != string::npos ||
        d.find("broken")   != string::npos ||
        d.find("joint")    != string::npos ||
        d.find("spine")    != string::npos ||
        d.find("back")     != string::npos ||
        d.find("knee")     != string::npos ||
        d.find("shoulder") != string::npos ||
        d.find("ortho")    != string::npos)   
        return "Orthopedic";

 // ENT keywords
    if(d.find("ear")      != string::npos ||
        d.find("nose")     != string::npos ||
        d.find("throat")   != string::npos ||
        d.find("tonsil")   != string::npos ||
        d.find("sinus")    != string::npos ||
        d.find("hearing")  != string::npos ||
        d.find("ent")      != string::npos)   
        return "ENT";

 // Radiologist keywords
    if(d.find("xray")     != string::npos ||
       d.find("x-ray")    != string::npos ||
       d.find("scan")     != string::npos ||
       d.find("mri")      != string::npos ||
       d.find("radio")    != string::npos ||
       d.find("imaging")  != string::npos ||
       d.find("ultrasound")!= string::npos)  
    return "Radiologist";

// Dentist keywords
    if(d.find("tooth")    != string::npos ||
        d.find("teeth")    != string::npos ||
        d.find("dental")   != string::npos ||
        d.find("gum")      != string::npos ||
        d.find("mouth")    != string::npos ||
        d.find("jaw")      != string::npos)   
        return "Dentist";
 
        return ""; // no match — will go to general ER queue
    }

    // insert Doctor record
    void insertDoctor(string dept, string name, int room){
        int idx = HashFunction(toLowerCase(dept));
    // Linear probing - find next free slot
    int count = 0;
    while(table[idx].occupied && count < SIZE){
        idx = (idx+1) % SIZE;
        count ++;
    }
    if (count == SIZE)
    { 
        cout<<"\nTable is full";
        return;
    }
    table[idx].department = dept;
    table[idx].name = name;
    table[idx].room = room;
    table[idx].occupied= true;
    table[idx].available = true;
    }
    // assign patient to doctor
    bool assignPatienttoDoctor(string dept, int PatientID, string PatientName, int priority, string disease, BST &database){
        int idx = HashFunction(toLowerCase(dept));
        int count =0;
        while(count < SIZE){
            if(table[idx].occupied && toLowerCase(table[idx].department)==toLowerCase(dept) && table[idx].available){
                table[idx].available= false;
                table[idx].treatingPatientID = PatientID;
                table[idx].treatingPatientName = PatientName;
                table[idx].treatingPriority= priority;
                table[idx].treatingDisease = disease;
                cout<<"\nDr. "<<table[idx].name << " (Room no: "<< table[idx].room << ") is now treating the paitent "<<PatientName << "(ID: " << PatientID << ") for " << disease <<endl;
                return true;
            }
            idx = (idx +1) % SIZE;
            count++;
        }
        cout<<"\n No available doctor in " << dept;
        cout<<". Patient added to ER waiting room\n";
        return false;
    }
   
    // Manual mark available
    void setDoctorAvailable(string doctorName, Priority &ERQueue, BST &datbase, BillingQueue &billing);

// Search Doctor by department
    bool SearchDoctor(string dept){
        int idx = HashFunction(toLowerCase(dept));
        int count = 0;
        Doctor* busyDoctor = NULL;
        Doctor*Available= NULL;
        while(count < SIZE){
            if(table[idx].occupied && toLowerCase(table[idx].department) == toLowerCase(dept)){
                if(table[idx].available){
              Available = &table[idx];
              break;
                }
                else{
                    busyDoctor= &table[idx];
            }
        }
            idx = (idx +1) % SIZE;
            count++;
        }
        if(Available){
              cout<<"\n---Doctor Found---"<<endl;
                cout<<"Department: "<< Available->department<< endl;
                cout<<"Doctor: "<<Available->name << endl;
                cout<<"Room No: "<< Available->room << endl;
                cout<<"Status: Available"<< endl;
                    return true;
        }
        if(busyDoctor){
            cout<< "\n Doctor Found";
            cout<<"\nDoctor:" << busyDoctor->name<<endl;
            cout<<"\nDepartment: " << busyDoctor->department<<endl;
            cout<< "Room no. "<< busyDoctor->room <<endl;
            cout<<"Status : Busy __ Currently treating Patient"<< endl;
            cout<<"\nTreating : "<<busyDoctor->treatingPatientName;
            cout<<"\nPatient ID: "<<busyDoctor->treatingPatientID;
            cout<<"\nDisease: "<<busyDoctor->treatingDisease;
            cout<<"\nCondition: ";
            // Show who doctor is treating
            switch(busyDoctor->treatingPriority){
                case 1: 
                cout<<"\nCritcal Paitent";
                break;
                case 2: 
                cout<<"\nSerious Patient";
                break;
                case 3:
                cout<<"\nModerate Patient";
                break;
                case 4:
                cout<<"\nMinor Patient";
                break;
            }
            cout<<"\n";
            return false;
        }
        cout<< " No doctor has found for department" << dept << endl;
         return false;
    }
       // Search Doctor by name
    void SearchDoctorByName(string doctorName){
        string target = toLowerCase(doctorName);
        bool found = false;
        for(int i = 0; i < SIZE; i++){
            if(!table[i].occupied || toLowerCase(table[i].name) != target) continue;
            found = true;
                cout << "\n--- Doctor Record ---\n";
                cout << "Doctor     : " << table[i].name       << "\n";
                cout << "Department : " << table[i].department  << "\n";
                cout << "Room No.   : " << table[i].room        << "\n";
                if(table[i].available){
                    cout << "Status: Available — not treating anyone right now\n";
                }
                 else {
                    cout << "Status     : Busy — currently treating a patient\n";
                    cout << "Treating   : " << table[i].treatingPatientName << "\n";
                    cout << "Patient ID : " << table[i].treatingPatientID   << "\n";
                    cout << "Disease    : " << table[i].treatingDisease      << "\n";
                    cout << "Condition  : ";
                    switch(table[i].treatingPriority){
                        case 1: cout << "Critical\n";  break;
                        case 2: cout << "Serious\n";   break;
                        case 3: cout << "Moderate\n";  break;
                        case 4: cout << "Minor\n";     break;
                        default: cout << "Unknown\n";
                    }
                }
                break;
        }
        if(found){
            return;
        }
        // Not found — show registered names so user knows what to type
        cout << "\nDoctor \"" << doctorName << "\" not found.\n";
        cout << "Registered doctors are:\n";
        for(int i = 0; i < SIZE; i++){
            if(table[i].occupied){
                cout << "  " << table[i].name
                     << " (" << table[i].department << ")\n";
        }
    }
    }

 // Free doctor manually after treatment
    void FreeDoctorByName(string doctorName, Priority &ERQueue, BST &database, BillingQueue &billing, AdmittedWards &wards);
};
//------------------------------------------------------------------------------------------------------------------------------------

// Now include the full struct definations
#include"Paitent.h"
#include"PaitentDatabase.h"
#include"ERWaitingRoom.h"
#include"Billing.h"
#include"Admitted.h"

// helper: fully reset a doctor slot
inline void resetDoctor(Doctor &d){
    d.available = true;
    d.treatingPatientID = -1;
    d.treatingPatientName = "";
    d.treatingDisease = "";
    d.treatingPriority = -1;
    d.bloodTestPending = false;
}

//  Send to billing -- build a billing record from bst data and enqueue it
inline void sendtoBilling(int pid, string pname, string disease, int priority, string doctorName, string dept, BST &database, BillingQueue &billing){
    float bp = 0;
    float sugar =0;
    float hemo = 0;
    int age = 0;
     BSTNode * node = database.searchPatient(pid);
     if(node){
        bp = node->data.bloodPressure;
        sugar = node->data.bloodSugar;
        hemo = node->data.hemoglobin;
        age = node->data.age;
        node->data.location = "Billing Counter";
        node->data.status = "At Billing Counter";
     }
     BillRecord bill = generateBill(pid, pname, age, disease, priority, doctorName, dept, bp, sugar, hemo, getDateInput());
     billing.enqueue(bill);
}
// assignNextPatient — called every time a doctor becomes free.
// Looks for the next patient in this order:
//   1. Priority 1 or 2 patients in ICU/OPD with status "Under Observation"
//      whose disease matches this doctor's department
//   2. Examination Ward patients with status "Under Observation"
//      whose disease matches this doctor's department
//   3. ER Waiting Room heap — find first patient whose disease matches dept

// BST inorder scan: find first p1/p2 patient in ICU/OPD awaiting doctor
BSTNode* findPriority12Patient(BSTNode*node, HashTable &ht, string dept){
    if (node == NULL){
        return NULL;
    }
     // Search left subtree first (inorder = lowest ID first)
    BSTNode* found = findPriority12Patient(node->left, ht, dept);
    if(found != NULL) return found;

    // check this node
    if((node->data.priority ==1 || node->data.priority == 2) &&
    (node->data.status == "Under Observation")){
        string matched = ht.matchDiseaseToDept(node-> data.disease);
        if(ht.toLowerCase(matched) == ht.toLowerCase(dept)){
            return node;
        }
    }
    // Search the right sub tree
    return findPriority12Patient(node->right, ht, dept);
}
// Find Examination ward patients whose disease match with department
// for priority 3 and 4 patients with abnormal test results
AdmittedPatient* findExamPatient(AdmittedWards &wards, HashTable &ht, string dept){
    for(int i =0; i <wards.ExamWard.capacity; i++){
        if(!wards.ExamWard.capacity){
            continue;
        }
        if(wards.ExamWard.beds[i].status == "Under Observation" && ht.toLowerCase(ht.matchDiseaseToDept(wards.ExamWard.beds[i].disease))== ht.toLowerCase(dept)){
            return &wards.ExamWard.beds[i];
        }
    }
    return NULL;
}
//Assign next patient to a specific free doctor slot
inline void assignNextPatient(HashTable &ht, int i, Priority &ERQueue, BST &database, AdmittedWards &wards){
    string dept = ht.table[i].department;
    // ── Step 1: Check for waiting priority 1 or 2 patients in ICU/OPD from BST ─
    BSTNode* urgent = findPriority12Patient(database.root, ht, dept);
    if(urgent != NULL){
        Patient &p = urgent->data;
        ht.table[i].available           = false;
        ht.table[i].treatingPatientID   = p.id;
        ht.table[i].treatingPatientName = p.name;
        ht.table[i].treatingPriority    = p.priority;
        ht.table[i].treatingDisease     = p.disease;
        ht.table[i].bloodTestPending = false;
        // Update Patient Location and status:
        p.status = "Under Treatment";
        p.location = "With " + ht.table[i].name + " - " + dept + "(Room " + to_string(ht.table[i].room) + ")";
        //Update ward table status too
        if(p.priority ==1){
            wards.ICU.updateStatus(p.id, "Under Treatment", ht.table[i].name);
        }
        if(p.priority == 2){
            wards.OPD.updateStatus(p.id, "Under Treatment", ht.table[i].name);
        }
        cout<<"\n Priority "<< urgent->data.priority << " patient: "<< p.name << "(ID: " << p.id << ") from "<< (urgent->data.priority ==1 ? "ICU" : "OPD") << "is assigned to doctor: "<< ht.table[i].name <<"(" << dept << ")"<< endl;
        return;
    }
    // Step 2: Exaination Ward patients waiting for doctor
    AdmittedPatient * examPt = findExamPatient(wards, ht, dept);
    if(examPt != NULL){
        ht.table[i].available = false;
        ht.table[i].treatingPatientID = examPt->id;
        ht.table[i].treatingPatientName = examPt->name;
        ht.table[i].treatingPriority = examPt->priority;
        ht.table[i].treatingDisease = examPt->disease;
        ht.table[i].bloodTestPending= false;

        // Update exam ward and Bst status

        wards.ExamWard.updateStatus(examPt->id, "Under Treatment", ht.table[i].name);

        BSTNode* node = database.searchPatient(examPt->id);
        if(node){
            node->data.status = "Under Treatment";
            node->data.doctorName = ht.table[i].name;
        }
          cout<<"\nDr."<<ht.table[i].name
            <<" assigned to Examination Ward patient "<<examPt->name
            <<" (ID:"<<examPt->id<<").\n";
        return;
    }

// Step 3: Look in the heap for priority 3 or 4 patients (Normal test result)
for(int j =0; j < ERQueue.size; j++){
    string matched = ht.matchDiseaseToDept(ERQueue.heap[j].disease);
    if(ht.toLowerCase(matched)== ht.toLowerCase(dept)){
        Patient chosen = ERQueue.heap[j];

        // Remove from heap: swap with the last element and re-heapify
        ERQueue.heap[j]= ERQueue.heap[ERQueue.size -1];
        ERQueue.size--;
        if(ERQueue.size > 0){
            ERQueue.DownHeapify(j);
            if(j>0){
                ERQueue.UpHeapify(j);
            }
        }
         // Assign to doctor
            ht.table[i].available           = false;
            ht.table[i].treatingPatientID   = chosen.id;
            ht.table[i].treatingPatientName = chosen.name;
            ht.table[i].treatingPriority    = chosen.priority;
            ht.table[i].treatingDisease     = chosen.disease;

             // Update location in BST
            BSTNode* node = database.searchPatient(chosen.id);
            if(node != NULL){
                node->data.location = "With " + ht.table[i].name +
                                      " - " + dept +
                                      " (Room " + to_string(ht.table[i].room) + ")";
                node->data.status = "Under Treatment";
                node->data.doctorName = ht.table[i].name;
    }
     cout << "\n ER patient " << chosen.name
                 << " (ID: " << chosen.id << ", "
                 << (chosen.priority == 3 ? "Moderate" : "Minor")
                 << ") removed from ER and assigned to Dr."
                 << ht.table[i].name << " (" << dept << ").\n";
            return;
}
}
 // ── Step 4: Nothing matched ────────────────────────────────────
    cout << "No matching patient for Dr." << ht.table[i].name
         << " (" << dept << "). Doctor is idle.\n";
}


// Free Doctor from patient manually for priority 1 and 2 patients
inline void HashTable::FreeDoctorByName(string doctorName, Priority &ERQueue, BST &database, BillingQueue &billing, AdmittedWards &wards){
   string target = toLowerCase(doctorName);
   for(int i =0; i <SIZE; i++){
    if(!table[i].occupied || toLowerCase(table[i].name)!= target){
        continue;
    }
    if(table[i].available){
            cout<<"\nDr."<<table[i].name<<" is already free.\n";
            return;
        }
       
        int pid = table[i].treatingPatientID;
        string pname = table[i].treatingPatientName;
        string dept = table[i].department;
        string disease = table[i].treatingDisease;

        cout<< "\n Dr. "<<table[i].name << " finished treating " << table[i].treatingPatientName << " (Priority: " << table[i].treatingPriority << ") and is now free and available."<<endl;
        cout<<"Department: "<< table[i].department<<endl;
        cout<<"Room no. : "<<table[i].room<<endl;
// Remove patient from their ward
        if(table[i].treatingPriority ==1 ){
            wards.ICU.removePatient(pid);
        }
       else if(table[i].treatingPriority ==2){
        wards.OPD.removePatient(pid);
       }
       else {
        wards.ExamWard.removePatient(pid);
       }
//Reset doctor slot
        resetDoctor(table[i]);

        sendtoBilling(pid, pname, disease, table[i].treatingPriority, table[i].name, dept, database, billing);
        assignNextPatient(*this, i, ERQueue, database, wards);
        return;
   }
    cout<<"\nDoctor \""<<doctorName<<"\" not found.\n";
    cout<<"Registered doctors:\n";
    for(int i=0;i<SIZE;i++)
        if(table[i].occupied)
            cout<<"  "<<table[i].name<<" ("<<table[i].department<<")\n";
}
#endif