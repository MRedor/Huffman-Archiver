#ifndef MY_HUFF_H
#define MY_HUFF_H

#include <cstdio>
#include <cstddef>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class HuffArchiver {
public:
    HuffArchiver(ifstream& fin, ofstream& fout);

    void code();
    void decode();

    void setInput(const string inp);
private:
    ifstream& fin;
    ofstream& fout;
    string inputname, outputname;
};

class TreeNode {
public:
    char key;
    TreeNode* l;
    TreeNode* r;

    TreeNode(char key);
    TreeNode(char key, TreeNode* l, TreeNode* r);
private:
};

class HuffTree {
public:
    HuffTree();
    HuffTree(TreeNode* v);
    ~HuffTree();

    void deleteTree(TreeNode* v);

    void printAlphabet(TreeNode* v, ofstream& fout);
    void printTree(TreeNode* v, ofstream& fout);
    void calcCodeTree(TreeNode* v);

    TreeNode* getRoot();
private: 
    TreeNode* root;
};


#endif