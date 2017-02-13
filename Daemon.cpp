#include <iostream>
#include <cstring>
#include <fstream>
#include "Predictor.h"

#define FILE_TARGET "/home/amitha/CLionProjects/BHTPerf/BranchPredictionSimulator/traces/"
#define _FILE_SAMPLE_ "sample"

void startSequencer(int predictorID, string fileName);

void invokePredictor(int predictorID, Predictor *p, int branch, int value);

using namespace std;


int main() {
    startSequencer(2, _FILE_SAMPLE_);
    return 0;
}

void invokePredictor(int predictorID, Predictor *p, int branch, int value) {
    bool result = p->takeBranch(predictorID, branch, value);
    cout << branch << " " << value << " " << (result ? "BRANCH" : "NO") << endl;
}

void startSequencer(int predictorID, string fileName) {
    cout << "Analyzing..." << endl << endl;
    Predictor *p1 = new Predictor(); //handle leak later
    string line;
    ifstream myFile((string) FILE_TARGET + fileName);
    if (myFile.is_open()) {
        while (getline(myFile, line)) {
            char *str = (char *) line.c_str();
            char *pch;
            pch = strtok(str, ", ");
            int branch = atoi(pch);
            pch = strtok(NULL, ", ");
            int value = atoi(pch);
            invokePredictor(predictorID, p1, branch, value); //calls the relevant Predictor
        }
        myFile.close();
    } else cout << "Unable to open file";
    p1->printStats();
}

