# 🏥 Built a Hospital Emergency Room Management System in C++

**A console-based C++ simulation of a hospital Emergency Room's patient workflow — from registration and triage through ward admission, doctor assignment, and billing.**

Built as a Data Structures & Algorithms course project to demonstrate practical, real-world use of core data structures beyond textbook examples.

#### Overview

The system models how a real ER handles patients:

Patients are registered and triaged by priority (1 = Critical, 4 = Minor)
Priority 1/2 patients are matched to a free doctor immediately, or admitted to the ICU/OPD if none is available
Priority 3 patients go to the Examination Ward; priority 4 patients join the ER Waiting Room
Every patient's vitals, status, and full visit history are tracked and searchable by ID or name
On discharge, the system auto-generates an itemized bill (doctor fee, treatment cost, medicine cost) with optional insurance discounts

#### Features
1) Patient registration with priority-based triage and automatic ward/doctor routing
2) Doctor management — assign, search, and free doctors by department
3) Ward tracking for ICU, OPD, and Examination Ward with fixed bed capacity
4) ER Waiting Room ordered by urgency, not arrival time
5) Persistent visit history per patient (supports returning patients)
6) Billing engine with itemized charges and insurance discount handling

#### Data Structures Used
Data Structure	Purpose	Reason
1) **Binary Search Tree (BST)**	Stores and retrieves patient records by ID	O(log n) search/insert — ideal for frequent ID lookups
2) **Min-Heap (Priority Queue)**	Orders ER Waiting Room patients	Always serves the most urgent patient first, in O(log n)
3) **Hash Table (linear probing)**	Tracks admitted beds and doctor records	Near O(1) access by ID/department for fast lookup
4) **Linked List**	Stores each patient's visit history	History length varies per patient, so dynamic growth fits best
5) **Queue (FIFO, linked list)**	Holds patients waiting for billing	Bills are processed in the same order patients finish treatment

#### Tech Stack
Language: C++17
Paradigm: Procedural, struct-based (no external libraries beyond the STL)
Build: Single-command compilation, no external dependencies
Project Structure
├── Main.cpp              # Entry point, menu loop, registration flow
├── Paitent.h              # Patient struct, vitals validation, visit history
├── PaitentDatabase.h      # BST for patient records
├── ERWaitingRoom.h        # Min-heap priority queue for the ER waiting room
├── Admitted.h             # Hash-table-based ward management (ICU/OPD/ExamWard)
├── Doctor.h               # Doctor records, department matching, assignment
└── Billing.h              # Billing queue, fee calculation, receipt generation

No external dependencies — compiles with just the C++ standard library.

#### Sample Workflow
Register a patient with a priority level (1–4)
The system automatically routes them to a doctor or the appropriate ward
Search for the patient by ID or name at any time to check status
Once treatment is complete, free the doctor to move the patient to billing
Process the bill to generate an itemized receipt
