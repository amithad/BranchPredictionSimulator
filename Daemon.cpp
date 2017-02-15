#include <iostream>
#include <cstring>
#include <fstream>
#include "Predictor.h"

#define FILE_TARGET "/home/amitha/CLionProjects/BHTPerf/BranchPredictionSimulator/traces/"
#define FILE_NAME "ray"
#define PREDICTOR_ID 4

void startSequencer(int predictorID, string fileName);

void invokePredictor(int predictorID, Predictor *p, unsigned int branch, int value);

bool printPredictorType(int predictorID);

using namespace std;


int main(int argc, char* argv[]) {
    //startSequencer(PREDICTOR_ID, FILE_NAME);
    startSequencer(atoi(argv[1]), argv[2]);
    return 0;
}

void invokePredictor(int predictorID, Predictor *p, unsigned int branch, int value) {
    bool result = p->takeBranch(predictorID, branch, value);
    //cout << branch << " " << value << " " << (result ? "BRANCH" : "NO") << endl;
}

void startSequencer(int predictorID, string fileName) {
    Predictor *p1 = new Predictor(); //handle leak later
    string line;
    ifstream myFile((string) FILE_TARGET + fileName);
    if (myFile.is_open()) {
        printPredictorType(predictorID);
        cout << "Analyzing..." << endl << endl;
        while (getline(myFile, line)) {
            char *str = (char *) line.c_str();
            char *pch;
            pch = strtok(str, ", ");
            unsigned int branch = (unsigned int) atoi(pch);
            pch = strtok(NULL, ", ");
            int value = atoi(pch);
            invokePredictor(predictorID, p1, branch, value); //calls the relevant Predictor
        }
        myFile.close();
    } else {
        cout << "Unable to locate file. Exiting.." <<endl;
        exit(-1);
    }
    p1->printStats();
}

bool printPredictorType(int predictorID) {

    switch (predictorID) {
        case 1:
            cout << "Branch Predictor: 8,192 BHT" << endl << endl;
            break;
        case 2:
            cout << "Branch Predictor: 2-bit 4,096 BHT" << endl << endl;
            break;
        case 3:
            cout << "Branch Predictor: (2, 2) 1024 BHT" << endl << endl;
            break;
        case 4:
            cout << "Branch Predictor: Custom Optimized" << endl << endl;
            break;
        default:
            cout << "Unidentified predictor type. Exiting.." << endl;
            exit(-1);
    }
}