#include<iostream>
#include<string>
using namespace std;
#include"Paitent.h"
#include"PaitentDatabase.h"
#include"ERWaitingRoom.h"
#include"Doctor.h"
#include"Admitted.h"
#include"Billing.h"

// REGISTRATION FUNCTION
//   1 (Critical) -> ICU        (doctor assigned immediately if one is free)
//   2 (Serious)  -> OPD        (doctor assigned immediately if one is free)
//   3 (Moderate) -> Examination Ward
//   4 (Minor)    -> ER Waiting Room (min-heap queue)
// If a ward is full, the patient falls back to the ER Waiting Room.
// Priority 1/2 skip vitals entry (too urgent); priority 3/4 enter vitals,
// which are recorded for the file but no longer change where they're sent.

void RegisterPatient(Patient &p, Priority &ERQueue, BST &database, HashTable &doctors, AdmittedWards &wards){
    string dept = doctors.matchDiseaseToDept(p.disease);
    if(p.priority == 1 || p.priority == 2){
        p.bloodPressure = 145.0f; // High
        p.bloodSugar = 55.0f; //Low
        p. hemoglobin = 9.0f; //low
        p.testsAbnormal = true;
    }
    else {
        // For moderate and Minor priority Patients
        cout<<"\nEnter Blood Pressure (Normal: 90-129)"<<endl;
        cin>>p.bloodPressure;
        cout<<"\nEnter Blood Sugar (mg/dL, Normal:70-140)"<<endl;
        cin>>p.bloodSugar;
        cout<<"\nEnter Hemoglobin (g/dL, Normal: 12.0-17.50)"<<endl;
        cin>>p.hemoglobin;

          p.testsAbnormal = CheckAbnormal(p.bloodPressure,p.bloodSugar,p.hemoglobin);
    }
    //print the results
    cout<<"\n Reading Summary"<<endl;
    cout<<"Blood Pressure: "<< p.bloodPressure <<"mmHg "<< getBPLabel(p.bloodPressure)<<endl;
    cout<<"Blood Sugar: "<<p.bloodSugar <<"mg/dL "<<getSugarLabel(p.bloodSugar)<<endl;
    cout<<"Hemoglobin: "<<p.hemoglobin<<"g/dL "<<getHemoLabel(p.hemoglobin)<<endl;

    if(p.priority ==1 || p.priority ==2){
        p.status = "Under Observation";
        bool assigned = false;
        if(dept!=""){
            assigned= doctors.assignPatienttoDoctor(dept,p.id, p.name,p.priority,p.disease,database);
            if(assigned){
                p.location = "With "+dept;
                p.status = "Under Treatment";
                p.doctorName =dept; // update in assignPatienttoDoctor via BST
            }
        }
        else {
            cout<<"\nNo specific department for this disease";
        }
        if(!assigned){
            if(p.priority ==1){
            p.location = "ICU";
        if(wards.ICU.isFull()){
            cout<<"\n ICU is FULL. Patient "<< p.name << " is shifted to ER queue";
            p.location = "ER WaitingRoom";
            ERQueue.Enqueue(p);
        }
        else {
            wards.ICU.admitPatient(p.id, p.name, p.age,p.disease,p.priority,"Under Observation", "");
            cout<<"Critical Patient. " << p.name<<"(ID: "<<p.id <<") is admitted in the ICU " <<endl;

        }
    }
    else {
        // priority 2
        p.location = "OPD";
        if(wards.OPD.isFull()){
            cout<<"\n OPD is Full. "<<p.name <<" (ID:"<<p.id<<")is in ER waiting room"<<endl;
            p.location= "ER waiting Room";
            ERQueue.Enqueue(p);
        }
        else{
            wards.OPD.admitPatient(p.id,p.name,p.age,p.disease,p.priority, "Under Observation", "");
            cout<<"\nSerious Patient. "<<p.name << " (ID:" << p.id <<") is admitted in the OPD "<<endl;
        }
    }
        }
    }
    
 // Only priority 3/4 patients go through the abnormal-test / ExamWard-or-ER-queue
    if(p.priority == 3){
        p.status = "Under Observation";
        if(wards.ExamWard.isFull()){
            cout<<"\n Examination Ward is Full." <<p.name<< " (ID "<< p.id << ") is shifted to ER waiting Room"<<endl;
            p.location = "ER waiting room";
            p.status = "Waiting";
            ERQueue.Enqueue(p);
        }
        else{
            p.location = "Examination Ward";
            wards.ExamWard.admitPatient(p.id,p.name,p.age,p.disease,p.priority,"Under Observation", "");
            cout<<"\nAbnormal Readings."<<p.name << "(ID:"<<p.id<<") is admitted to the Examination Ward."<<endl;
            if(dept!=""){
                bool ok = doctors.assignPatienttoDoctor(dept,p.id,p.name,p.priority,p.disease,database);
                if(ok){
                    p.status = "Under Treatment";
                    wards.ExamWard.updateStatus(p.id,"Under Treatment",dept);
                }
            }
            }
        }
        
    
    else if (p.priority == 4) {
        // Normal readings -> ER waiting Room
        p.location = "ER Waiting Room";
        p.status = "Waiting";
        ERQueue.Enqueue(p);
        cout<<"\nPatient " <<p.name <<" (ID:"<< p.id << ") added to ER waiting Room"<<endl;
        if(dept!=""){
            cout<<"(will be routed to " << dept<<")"<<endl;
        }
    }
    // Save to BST
    database.insertPatient(p);
    BSTNode* node = database.searchPatient(p.id);
    if(node){
        node->data.location = p.location;
        node->data.status = p.status;
        node->data.bloodPressure = p.bloodPressure;
        node->data.bloodSugar = p.bloodSugar;
        node->data.hemoglobin = p.hemoglobin;
        node->data.testsAbnormal = p.testsAbnormal;
        string visitDate = getDateInput();
        node->history.AddVisit(p.disease, visitDate ,p.name);
    }
}

int main(){
    // Intialize test Thresholds (normal reference values)
    THRESHOLD.setDefaults();
    BillingQueue billing;
    AdmittedWards wards;
    Priority ERQueue;
    BST database;
    HashTable doctors;
    int choice;

    // pre insert Doctors in the system
    doctors.insertDoctor("Cardiology", "Dr.Ahmed", 3);
    doctors.insertDoctor("Neurology", "Dr.Sarah", 1);
    doctors.insertDoctor("Orthopedic", "Dr.Maha",2);
    doctors.insertDoctor("ENT", "Dr.Kayyam", 15);
    doctors.insertDoctor("Radiologist", "Dr.Omar",20);
    doctors.insertDoctor("Dentist", "Dr.Zara", 22);

    cout<<"----Hospital Emergency Room Management System---- \n";
    while(true){
        cout<<"\n-------------Menu--------------";
        cout<<"\n1.Register a new Patient";
        cout<<"\n2.Search Patient by ID";
        cout<<"\n3.Search Patient by name"; 
        cout<<"\n4.View paitent visit history";
        cout<<"\nER WAITING ROOM";
        cout<<"\n5.View all waiting paitents";
        cout<<"\n6.Manually admit next ER Patient";
        cout<<"\nDOCTOR";
        cout<<"\n7.Search doctor by name";
        cout<<"\n8.Search doctor by department";
        cout<<"\n9.Set free Doctor";
        cout<<"\n ADMITTED WARDS";
        cout<<"\n10.View ICU Patients";
        cout<<"\n11.View OPD Patients";
        cout<<"\n12.View Examination Ward Patients";
        cout<<"\n13.View All admitted Patients";
        cout<<"\n14. Search Admitted Patient by Name";
        cout<<"\nBILLING";
        cout<<"\n15.View Billing Queue";
        cout<<"\n16.Process next bill";
        cout<<"\n------------------------------";
        cout<<"\n17.Program ended";

        cout<<"\nEnter your choice: ";
        cin>>choice;
        switch(choice){

            case 1:{
            // Registering patient (new or returning):
            Patient p;
            cout<<"\n Assign patient ID: ";
             cin>> p.id;
             BSTNode* existing = database.searchPatient(p.id);
             if(existing){
                cout<<"\n Exisiting patient found:\n Patient Name: "<< existing->data.name <<"\n Patient Age: " << existing->data.age<<"\n Recording this as a new visit."<<endl;
                p.name = existing->data.name;
                p.age = existing->data.age;
             }
             else{
            cout<<"\nEnter Patient Name: ";
            cin.ignore();
            getline(cin, p.name);
            cout<<"\nEnter Patient Age: ";
            cin>>p.age;
            }
            cout<<"\nEnter Disease: ";
            cin.ignore();
        getline(cin, p.disease);
        cout<<"\nEnter Paitent Condition Severity (priority): ";
        cin>>p.priority;
        RegisterPatient(p, ERQueue, database, doctors, wards);
    cout<<"Paitent: "<< p.id << "\nRegistered Sucessfully"<< endl;
    break;
            }
            // 2. Search Patient by ID
        case 2: {
            int sid;
            cout<<"\nEnter patient ID: "<<endl;
            cin>>sid;
            BSTNode* curr = database.searchPatient(sid);
            if(curr){
                Patient &d = curr->data;
                cout<<"\n--Patient Record--"<<endl;
                cout<<"\nID: "<<d.id<<endl;
                cout<<"\nName: "<<d.name<<endl;
                cout<<"\nAge: "<<d.age<<endl;
                cout<<"\nDisease: "<<d.disease<<endl;
                cout<<"\nPriority: ";
                switch(d.priority){
                    case 1: cout<<"\nCritical"; break;
                    case 2: cout<<"\nSerious"; break;
                    case 3: cout<<"\nModerate"; break;
                    case 4: cout<<"\nMinor"; break;
                }
                if(d.bloodPressure>0){
                    cout<<"\n Blood Pressure: "<< d.bloodPressure <<"mmHg ["<< getBPLabel(d.bloodPressure)<< "]"<<endl;
                    cout<<"\n Blood Sugar: "<<d.bloodSugar<<"mg/dL ["<<getSugarLabel(d.bloodSugar)<< "]"<<endl;
                    cout<<"\nHemoglobin: "<<d.hemoglobin<<"g/dL["<<getHemoLabel(d.hemoglobin)<< "]"<<endl;
                }
                cout<<"\nLocation: "<<d.location<<endl;
                cout<<"\nStatus: "<<d.status<<endl;
                if(!d.doctorName.empty())
                cout<<"Doctor: "<<d.doctorName<<endl;
            }
            else{
                cout<<"\nPatient ID"<<sid <<" is not found"<<endl;
            }
            break;
        }
        
        case 3: {
        //Search Paitent by Name
        string sname;
        cout<<"Enter the patient name: ";
        cin.ignore();
        getline(cin, sname);

        BSTNode* curr = database.searchPatientByName(sname);
        if(curr != NULL){
            Patient d = curr->data;
            cout<<"\n--Patient Record--";
            cout<<"\nPatient ID: "<<d.id << endl;
            cout<<"Patient Name: "<<d.name<< endl;
            cout<<"Patient Age: "<<d.age<<endl;
            cout<<"Patient Disease: "<<d.disease<<endl;
            cout<<"Paitent's Location: "<<d.location<<endl;
            cout<<"Status: "<<d.status<<endl;
            if(!d.doctorName.empty()){
                cout<<"Doctor: "<<d.doctorName<<endl;
            }
        }
        else{
            cout<<"\nPatient does not found";
        }
        break; 
}

        case 4: {
        // Check visit history
        int PatientID;
        cout<<"\nEnter the Patient ID to Search the visit History: ";
        cin>>PatientID;

        BSTNode*curr = database.searchPatient(PatientID);
        if( curr != NULL){
            cout<<"\nShowing Patient No. "<< PatientID <<" Record History: "<<endl;
            curr->history.display();
        }
        else {
            cout<<"\n Patient not found";
        }
        break;
    }
        case 5: {
        // View All Waiting Patients
        cout<<"\n Current State of ER Waiting Room : ";
        ERQueue.ERdisplay();
        break;
    }
    case 6: {
        // Manual admit from ER
        if(ERQueue.isEmpty()){
            cout<<"\nER is empty";
            break;
        }
        Patient admitted = ERQueue.Dequeue();
        string dept = doctors.matchDiseaseToDept(admitted.disease);
        cout<<"\nAdmitted "<<admitted.name<< " (ID:"<<admitted.id<<")";
        if(dept!=""){
            bool ok= doctors.assignPatienttoDoctor(dept, admitted.id,admitted.name, admitted.priority, admitted.disease, database);
            BSTNode* node = database.searchPatient(admitted.id);
            if(ok && node){
                node->data.location = "With Dr. "+ dept;
                node->data.status ="Under Treatment";
            }
            else if(node){
                node->data.location = "Ward";
                node->data.status = "Waiting";
                cout<<"No other doctor in "<<dept<<" department. Patient is send to ER";

            }
        }
        else{
            cout<<"\nPatient send to ward";
            BSTNode*node = database.searchPatient(admitted.id);
            if(node){
                node->data.location = "Ward";
                node->data.status = "waiting";
            }
        }
       
           break;
        }
   
        case 7:{
       // Search doctor by name
       string dname;
       cout<<"Enter Doctor name:";
       cin.ignore();
       getline(cin, dname);
       doctors.SearchDoctorByName(dname);
       break;
        }
    case 8 :{
            string dept;
            cout<<"Enter Department: ";
            cin.ignore();
            getline(cin, dept);
            doctors.SearchDoctor(dept);
            break;
        }
        case 9 :
        // Free Doctor Manually
        {
            string doctorName;
            cout<<"Enter the Doctor name to set free: ";
            cin.ignore();
            getline(cin, doctorName);
            doctors.FreeDoctorByName(doctorName, ERQueue, database, billing,wards);
            break;
        }
        case 10:
        {
        wards.displayICU();
        break;
        }
        case 11:
        {
            wards.displayOPD();
            break;
        }
        case 12:
        {
            wards.displayExamWard();
            break;
        }
        case 13:
        {
            wards.DisplayAll();
            break;
        }
        // Search admitted patient by name
        case 14:
        {
            string sname;
            cout<<"Enter Patient Name: ";
            cin.ignore();
            getline(cin, sname);
            wards.searchbyName(sname);
            break;
        }
        // Billing Queue
        case 15:
        {
            viewbillingQueue(billing);
            break;
        }
        // Process bill
        case 16:
        {
            processBill(billing, database);
            break;
        }
        // Exit
        case 17:
        {
            cout<<"\nProgram Ended";
            exit(0);
        }
        default:
        cout<<"Invalid Choice";
        break;
        }
    }
    return 0;
}