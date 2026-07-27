#ifndef BILLING_H
#define BILLING_H

// Billing System using Queue (FIFO)
// Patients join billing in order they finish treatment
// First in = first out
# include<iostream>
#include<string>
#include<iomanip>

using namespace std;
#include"Paitent.h"
#include"PaitentDatabase.h"

// Doctor Fees by department
// Fix consultation fees per department
// Called once per bill based on which department treated the patient
//------------------------------------------------------------------------------------

int getDoctorFee(string department){
    string d = department;
    for(int i =0; i<(int)d.size(); i++){
        d[i] = tolower(d[i]);
    }
    if(d == "cardiology"){
        return 1300;
    }
    if(d == "neurology"){
        return 1500;
    }
    if(d == "orthopedic"){
        return 1000;
    }
    if(d == "ent"){
        return 500;
    }
    if(d == "radiology"){
        return 1000;
    }
    if(d == "dentist"){
        return 1200;
    }
    return 200;
}
// Treatment cost by Disease
// Fixed procedure/treatment fee based on disease keywords

int getTreatmentCost(string disease){
    string d = disease;
    for(int i =0; i<(int)d.size(); i++){
        d[i] = tolower(d[i]);
    }
    if(d.find("heart")!=string::npos || d.find("cardiac")!=string::npos || d.find("attack")!=string::npos){
        return 1400;
    }
    if(d.find("brain")!=string::npos || d.find("stroke")!=string::npos || d.find("migraine")!=string::npos){
        return 1000;
    }
    if(d.find("fracture")!=string::npos || d.find("broken")!=string::npos){
        return 800; 
    }
    if(d.find("tumor")!=string::npos || d.find("seizure")!=string::npos){
        return 1000;
    }
    if(d.find("bone")!=string::npos || d.find("joint")!=string::npos){
        return 800;
    }
    if(d.find("ear")!=string::npos || d.find("sinus")!=string::npos || d.find("throat")!=string::npos){
        return 200;
    }
    if(d.find("tooth")!=string::npos || d.find("dental")!= string::npos){
        return 800;
    }
    if(d.find("fever")!=string::npos || d.find("flu")!=string::npos || d.find("cold")!=string::npos){
        return 100;
    }
    if(d.find("stomach")!=string::npos || d.find("gastric")!=string::npos){
        return 300;
    }
    return 150; // general
}

// Medicine cost by disease
//Listed seprately on the bill so the patient can see it.

int getMedicineCost(string disease){
    string d = disease;
    for(int i=0;i<(int)d.size(); i++){
        d[i] = tolower(d[i]);
    }
    if(d.find("heart")!=string::npos||d.find("cardiac")!=string::npos|| d.find("attack")!=string::npos) {
         return 200;    }
    if(d.find("brain")!=string::npos||d.find("stroke")!=string::npos|| d.find("tumor")!=string::npos) {
            return 180; }
    if(d.find("fracture")!=string::npos||d.find("broken")!=string::npos){
       return 120.0f;}
    if(d.find("surgery")!=string::npos) { 
         return 300; }
    if(d.find("migraine")!=string::npos||d.find("seizure")!=string::npos){
      return  80; }
    if(d.find("bone")!=string::npos||d.find("joint")!=string::npos||d.find("knee")!=string::npos) {      
         return  70; }
    if(d.find("ear")!=string::npos||d.find("throat")!=string::npos||d.find("sinus")!=string::npos){
        return  40;}
    if(d.find("tooth")!=string::npos||d.find("dental")!=string::npos){
    return  35; }
    if(d.find("fever")!=string::npos||d.find("flu")!=string::npos||d.find("cold")!=string::npos){
         return  20; }
    if(d.find("stomach")!=string::npos||d.find("gastric")!=string::npos){
    return  30; }
    return 15; // general
}
// Now for insurance 
// Normal Card = 50% discount
// VIP card = 80% discount
// No Card = 0% discount

float getInsuranceDiscount(string cardType, float TotalAmount){
    string c = cardType;
    for(int i =0;i<(int)c.size();i++){
        c[i]= tolower(c[i]);
    }
    if(c == "vip"){
        return TotalAmount * 0.80f;
    }
    if(c =="normal"){
        return TotalAmount * 0.50f;
    }
    return 0.0f; // for no card
}

// Bill Record _ everything that appears on the bill

struct BillRecord{
    // Patient info
    int    patientID;
    string patientName;
    int    patientAge;
    string disease;
    int    priority;
    string doctorName;
    string department;
    string date;

    // Test results (printed with high/ low/ Normal)
    float bloodPressure;
    float bloodSugar;
    float hemoglobin;

    // Costs — all flat fixed amounts, simple and clear
    int doctorFee;         // fixed by department
    int treatmentCost;     // fixed by disease
    int medicineCost;      // flat medicine charge by disease
    int subtotal;          // doctorFee + treatmentCost + medicineCost
    float insuranceDiscount; // amount discounted (0 if no insurance)
    float totalBill;         // what the patient actually pays

    BillRecord(){
        patientID = -1;
        patientName = "";
        patientAge = 0;
        priority = 0;
        bloodPressure = 0;
        bloodSugar = 0;
        hemoglobin = 0;
        doctorFee = 0;
        treatmentCost = 0;
        medicineCost = 0;
        subtotal = 0;
        insuranceDiscount = 0;
        totalBill = 0;
    }
};

    // Billing Queue - Dynamic Linked List Queue (FIFO)
    // Each node hold one bill Record and pointer to the next
    // front pointer -> next to be billed (dequeue from here)
    // Rear pointer -> last in line (enqueue from here)
    // when queue is empty both front and rear ends are NULL

    struct BillNode {
        BillRecord data; // bill store in this node
        BillNode*next;
        BillNode(BillRecord b){
            data = b;
            next = NULL;
        }
};
struct BillingQueue{
    BillNode* front;
    BillNode* rear;
    int count; // count how many patients are waiting
    BillingQueue(){
        front = NULL;
        rear = NULL;
        count =0;
    }
    bool isEmpty(){
     return front == NULL;
}
bool isFull(){
    return rear == front;
}
void enqueue(BillRecord b){
    BillNode* Newnode = new BillNode(b);
    if(rear == NULL){
        // The queue is new and empty
        front=rear= Newnode;
    }
    else{
        rear->next = Newnode;
        rear = Newnode;
    }
    count ++;
    cout<<"Patient "<< b.patientName << "("<<b.patientID <<") has been added to the billing queue, position: "<< count<<endl;
}
// Dequeue patient once billing is done and only insurance is left
BillRecord dequeue(){
    BillRecord b = front->data;
    BillNode*temp = front;
    front = front->next;
    if(front == NULL){
        rear = NULL;
    }
    delete temp;
    count--;
    return b;
}
// Display everyone waiting in the queue
void displayQueue(){
    if(isEmpty()){
        cout<<"\n Queue is empty";
        return;
    }
    cout<< "\n ----BillingQueue----";
    cout<< "\nNo. of Patients in the queue: "<< count<< endl;
BillNode*curr = front;
int pos = 1;
while(curr != NULL){
    cout<< pos << "."<< "PatientID: " <<curr->data.patientID<<endl;
    cout<<"PatientName: "<< curr->data.patientName<<endl;
    cout<<"Disease: "<< curr->data.disease;
    pos++;
    curr = curr->next;
}
}
};
// Generate Bill
BillRecord generateBill(int pid, string pname, int age, string disease, int priority, string doctorName, string department, float bp, float sugar, float hemo, string date){
    BillRecord bill;
    bill.patientID = pid;
    bill.patientName = pname;
    bill.patientAge = age;
    bill.disease = disease;
    bill.priority = priority;
    bill.doctorName = doctorName;
    bill.bloodPressure = bp;
    bill.bloodSugar = sugar;
    bill.hemoglobin = hemo;
    bill.department= department;
    bill.date = date;
    // fixed fees
    bill.medicineCost = getMedicineCost(disease);
    bill.treatmentCost = getTreatmentCost(disease);
    bill.doctorFee = getDoctorFee(department);

    //Subtotal before insurance

    bill.subtotal = bill.doctorFee + bill.treatmentCost + bill.medicineCost;

    // Insurance applied in process Bill after asking user
    bill.totalBill = bill.subtotal;
    return bill;
}
// Process Bill - apply insurance discount after asking user
void processBill(BillingQueue &bq, BST &database){
    if(bq.isEmpty()){
        cout<<"\nNo patients in billing queue";
        return;
    }
    BillRecord bill = bq.dequeue();

    // Ask about insurance
    cout<<"\n Insurance Check for "<< bill.patientName<<"endl";
    cout<<" \n Do you have a health insurance (1 = yes, 2 = no): ";
    int hasInsurance;
    cin>>hasInsurance;
    string cardType = "none";
    if(hasInsurance){
        cout<<"Enter Insurance Type (vip/ normal)";
        cin>>cardType;
        bill.insuranceDiscount = getInsuranceDiscount(cardType, bill.subtotal);
        bill.totalBill = bill.subtotal - bill.insuranceDiscount;
        if(bill.totalBill < 0){
            bill.totalBill = 0;
        }
    }
        else{
            bill.totalBill = bill.subtotal;
        }

        // Display
        cout<<"\n---- HOSPITAL _ BILLING RECIPT-----"<<endl;
        cout<<"Date: "<< bill.date<<endl;
        cout<<"Patient ID: "<< bill.patientID<<endl;
        cout<<"Patient Name: "<< bill.patientName<<endl;
        cout<<"Age: " << bill.patientAge<<endl;
        cout<<"Disease: "<<bill.disease<<endl;
        cout<<"Condition: ";
        switch(bill.priority){
            case 1:
            cout<<"Critical"<<endl; break;
            case 2:
            cout<<"Serious"<<endl; break;
            case 3:
            cout<<"Moderate"<<endl; break;
            case 4:
            cout<<"Minor"<<endl; break;
        }
        cout<<"\nAttending Doctor: "<<bill.doctorName<<endl;
        cout<<"Department: "<<bill.department<<endl;
        cout<<"\n-----Tests Report------"<<endl;
        cout<<"Blood Pressure: "<< bill.bloodPressure<<" mmHg ["<< getBPLabel(bill.bloodPressure)<<"]"<<endl;
        cout<<"Sugar Level: "<<bill.bloodSugar<<" mg/dL [" << getSugarLabel(bill.bloodSugar)<<"]"<<endl;
        cout<<"Hemoglobin Level: "<<bill.hemoglobin<<" g/dL ["<<getHemoLabel(bill.hemoglobin)<<"]"<<endl;

        cout<<"\n----Charges Breakdown-----"<<endl;
        cout<<"Doctor Fee: "<<bill.doctorFee<<"pkr"<<endl;
        cout<<"Treatment Charges: "<<bill.treatmentCost<<"pkr"<<endl;
        cout<<"Medicine Cost: "<<bill.medicineCost<<"pkr"<<endl;
        cout<<"Medicine dispensed at pharmacy on exit"<<endl;
        cout<<"\n------------Total--------------";
        cout<<"\nSubTotal: "<<bill.subtotal<<endl;
        if(hasInsurance){
            cout<<"\nInsurance (" << cardType << ")"<<endl;
            cout<<"Insurance Applied Successfully \n"<<"Discount: "<<bill.insuranceDiscount<<endl;
        }
        cout<<"PAYABLE AMOUNT: " << bill.totalBill<<"pkr"<<endl;
        cout<<"\n Collect medicine at pharmacy. Thankyou"<<endl;
        cout<<"-----------------------------";

    
    // Auto save the visit to patient's history

BSTNode* node = database.searchPatient(bill.patientID);
if(node){
    node->history.AddVisit(bill.disease, bill.date, bill.patientName);
    node->data.location = "Billed and Discharged";
    node->data.status = "Discharged";
    cout<<"\n Visit successfully added to patient's file";
}
    }
    void viewbillingQueue(BillingQueue &bq){
        bq.displayQueue();
    }



#endif

   