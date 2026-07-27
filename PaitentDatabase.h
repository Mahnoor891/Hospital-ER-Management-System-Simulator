#ifndef PAITENTDATABASE_H
#define PAITENTDATABASE_H
// BY USING BINARY SEARCH TREE
// Here we will store paitent under unique id in sorted manner so that searching will be easy
//---------------------------------------------------------------------------------------------
#include<iostream>
using namespace std;
#include"Paitent.h"
 
struct BSTNode {
    Patient  data;
    PatientHistory history;
    BSTNode * left;
    BSTNode* right;
};
struct BST {
BSTNode* root;

BST() {
    root = NULL;
}
// Insert Paitent by ID
BSTNode* insert(BSTNode* curr, Patient p){
    if(curr == NULL){
        BSTNode*temp = new BSTNode();
        temp->data = p;
        temp->left = NULL;
        temp->right = NULL;
        return temp;
    }
    if(p.id == curr->data.id){
        curr->data =p;
        return curr;
    }
    if(p.id < curr->data.id){
        curr->left = insert(curr->left, p);
    }
    else {
        curr->right = insert(curr->right, p);
    }
    return curr;
}
//Public insert - called from main
void insertPatient(Patient p){
    root = insert(root, p);
}
// Now Search the Patient via ID
BSTNode*Search(BSTNode*curr, int id){
    if(curr == NULL){
        return NULL;
    }
    if(id == curr->data.id){
        return curr;
    }
    if(id< curr->data.id){
        return Search(curr->left, id);
    }
    else {
        return Search(curr->right, id);
    }
}
BSTNode*searchPatient(int id){
    return Search(root, id);
}
// Search patient by name - inorder traversal
BSTNode* SearchbyName(BSTNode*curr, string name){
    if(curr == NULL){
        return NULL;
    }
        BSTNode * found = SearchbyName(curr->left, name);
        if(found){
            return found;
        }
        // Case insensitive compare
        string a = curr->data.name;
        string b = name;
        for(int i =0; i<(int)a.size(); i++){
            a[i] = tolower(a[i]);
        }
        for(int i =0; i<(int)b.size(); i++){
            b[i] = tolower(b[i]);
        }
        if(a == b){
            return curr;
        }
        return SearchbyName(curr->right, name);
    }
    BSTNode*searchPatientByName(string name){
         return SearchbyName(root, name); 
        }
};
#endif