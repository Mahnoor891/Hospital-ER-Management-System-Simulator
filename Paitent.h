#ifndef PATIENT_H
#define PATIENT_H

#include<iostream>
#include<string>
using namespace std;

//Default Test Thresholds
// These are the reference values we compare patient readings against.
// If a reading is outside these ranges, it is flagged as abnormal.
//-------------------------------------------------------------------
struct TestThreshold{
    // Blood pressure (systolic, mmHg)
    float bp_low;  // below this = low BP (Hypotension)
    float bp_high; // above this = High BP (HyperTension)
    // Blood Sugar (mg/dL, non-fasting general)
    float sugar_low;    // below this = Low Sugar (Hypoglycemia)
    float sugar_high;   // above this = High Sugar (Hyperglycemia)
    // Hemoglobin (g/dL)
    float hemo_low;     // below this = Low Hemoglobin (Anemia)
    float hemo_high;    // above this = High Hemoglobin (Polycythemia)

        // Set default normal reference values
    void setDefaults(){
        bp_low    = 90.0f;
        bp_high   = 130.0f;
        sugar_low = 70.0f;
        sugar_high= 140.0f;
        hemo_low  = 12.0f;
        hemo_high = 17.5f;
    }
};

TestThreshold THRESHOLD; // object to use everywhere

// PATIENT'S RECORD
//----------------------------------------------------------------
struct Patient{
    int id;  // Patient's unique id
    string name; // patient name
    int priority; // How severe is one's condition
    string disease; // what is problem, paitent is suffering from
    int age; // patient's age
    string location; // Either waiting room, icu or opd or examination ward or er waiting area
    string status; // Current status: Waiting / Under Observation / Under Treatment / Billed & Discharged
    string doctorName;   // Name of the doctor assigned (empty if none yet)
 
    // Test readings — taken only for priority 2, 3 & 4 at registration
    // Priority 1 go directly to ICU/OPD; tests are done there
    float bloodPressure;  // Systolic BP in mmHg
    float bloodSugar;     // Blood sugar in mg/dL
    float hemoglobin;     // Hemoglobin in g/dL
    bool  testsAbnormal;  // true if any reading was outside normal range

    Patient() { // Default values
         id            = 0;
        age            = 0;
        priority       = 4;
        location       = "ER Waiting Room";
        status         = "Waiting";
        doctorName     = "";
        bloodPressure  = 0.0f;
        bloodSugar     = 0.0f;
        hemoglobin     = 0.0f;
        testsAbnormal  = false;
    }
};
// Check if tests results are Abnormal
// Returns true if any reading falls outside the normal thresholds.
// Used at registration to decide: Examination Ward vs ER Waiting Room.
//----------------------------------------------------------------------
bool CheckAbnormal(float bp, float sugar, float hemo){
    if(bp < THRESHOLD.bp_low || bp > THRESHOLD.bp_high){
        return true;
    }
     if(sugar < THRESHOLD.sugar_low || sugar >  THRESHOLD.sugar_high){ 
        return true;
    }
    if(hemo  < THRESHOLD.hemo_low  || hemo  >  THRESHOLD.hemo_high) {
         return true;
    }
    return false;
}
// GET LABEL FOR A SINGLE READING  ("Normal", "High", or "Low")
//---------------------------------------------------------------------
string getBPLabel(float bp){
    if(bp < THRESHOLD.bp_low){
        return "LOW";
    }
    if(bp >= THRESHOLD.bp_high){
        return "HIGH";
    }
    return "NORMAL";
}
string getSugarLabel(float sugar){
    if(sugar < THRESHOLD.sugar_low){
        return "LOW";
    }
    if(sugar >= THRESHOLD.sugar_high){
        return "HIGH";
    }
    return "NORMAL";
}
string getHemoLabel(float hemo){
    if(hemo < THRESHOLD.hemo_low){
        return "LOW";
    }
    if(hemo >= THRESHOLD.hemo_high){
        return "HIGH";
    }
    return "NORMAL";
}
string getDateInput(){
    int daysInMonth[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int day, month, year;
    while(true){
        cout<<"\nEnter date: ";
        cin>>day>>month>>year;

        if(year < 1900 || year > 2100){
            cout<<"\nInvalid year. Please enter a year between 1900 and 2100";
            continue;
        }
        if(month < 1 || month > 12){
            cout<<"\nInvalid month. Please enter a month between 1 and 12";
            continue;
        }
        if(day < 1 || day > daysInMonth[month -1]){
            cout<<"\nInvalid day for the month";
            continue;
        }
        break;
    }
    string dayStr = to_string(day);
    if(day < 10){
        dayStr = "0" + dayStr;
    }
    string Month = to_string(month);
    if(month < 10){
        Month = "0" + Month;
    }
    string date = dayStr + "/" + Month + "/" + to_string(year);
    return date;
}
 
// PATIENT'S HISTORY RECORD (using Linked List)
//-----------------------------------------------------------------
struct VisitRecord{
    string name;
    string disease;
    string date;
    VisitRecord* next;
};
struct PatientHistory{
    VisitRecord*head;

    PatientHistory(){
        head = NULL;
    }
// Now adding the latest visit at first
void AddVisit(string disease, string date, string name){
    VisitRecord*temp = new VisitRecord();
    temp->name = name;
    temp->disease = disease;
    temp->date = date;
    temp->next = NULL;
    if(head == NULL){
    head = temp;
    }
    else {
        VisitRecord* curr = head;
        while(curr->next!=NULL){
            curr = curr->next;
        }
        curr -> next = temp;
    }
}
// Display all the visits
void display(){
    if(head == NULL){
        cout<<"\n NO visit history";
        return;
    }
    VisitRecord*curr = head;
    int count =1;
    while(curr!= NULL){
        cout<< " Visit " << count++ << "\n Patient name: "<< curr->name <<"\n : [ " << curr->disease << " |Date: " << curr->date << " ] " << endl;
        curr= curr->next;
    }
}
};
#endif
