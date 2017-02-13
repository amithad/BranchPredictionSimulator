//
// Created by amitha on 2/13/17.
//


#include "Predictor.h"
#include <iostream>
#include <algorithm>

#define BHT_SIZE 8192
#define BHT_TWO_BIT_SIZE 4096

Predictor::Predictor() {
    for (int i = 0; i < BHT_SIZE; i++) {
        BHT.insert(pair<int, int>(i, 0));
    }
    for (int i = 0; i < BHT_TWO_BIT_SIZE; i++) {
        BHT2Bit.insert(pair<int, int>(i, 0));
    }
}

bool Predictor::takeBranch(int predictorID, int branch, int value) {

    switch (predictorID) {
        case 1:
            return BHTOneBit(branch, value);
        case 2:
            return BHTTwoBit(branch, value);
        case 3:
            return BHTTwoByTwo(branch, value);
        case 4:
            return BHTOptimized(branch, value);
        default:
            cout << "Unidentified BHT type. Exiting.." << endl;
            exit(-1);
    }
}

bool Predictor::BHTOneBit(int branch, int value) {
    totalBranches++;
    findIfUnique(branch); //stat collector call

    int pos = branch % BHT_SIZE;
    int result = BHT.at(pos);
    BHT[pos] = value;

    if (result == value) {
        correctPredictions++;
    } else {
        incorrectPredictions++;
    }

    return result == 1;
}

bool Predictor::BHTTwoBit(int branch, int value) {
    totalBranches++;
    findIfUnique(branch); //stat collector call

    int pos = branch % BHT_TWO_BIT_SIZE;
    int result = BHT2Bit.at(pos);
    if (result == 0) { //state: 00
        if (value == 1) {
            BHT2Bit[pos] = 1;
            incorrectPredictions++;
        } else
            correctPredictions++;
        return false;
    }
    if (result == 1) { //state: 01
        if (value == 1) {
            BHT2Bit[pos] = 3;
            incorrectPredictions++;
        } else {
            BHT2Bit[pos] = 0;
            correctPredictions++;
        }
        return false;
    }
    if (result == 2) { //state: 10
        if (value == 1) {
            BHT2Bit[pos] = 3;
            correctPredictions++;
        } else {
            BHT2Bit[pos] = 0;
            incorrectPredictions++;
        }
        return true;
    }
    if (result == 3) { //state: 11
        if (value == 1) {
            correctPredictions++;
        } else {
            BHT2Bit[pos] = 2;
            incorrectPredictions++;
        }
        return true;
    }
}

bool Predictor::BHTTwoByTwo(int branch, int value) {
    return false;
}

bool Predictor::BHTOptimized(int branch, int value) {
    return false;
}

void Predictor::printStats() {
    cout << "============================================================" << endl;
    cout << "\tTotal no of branches\t\t\t\t\t: " << totalBranches << endl;
    cout << "\tNo of unique branches\t\t\t\t\t: " << branches.size() << endl;
    cout << "\tNo of branches correctly predicted\t\t: " << correctPredictions << endl;
    cout << "\tNo of branches incorrectly predicted\t: " << incorrectPredictions << endl;
    float misPredictionRate = (float) (incorrectPredictions * 1.0 / totalBranches);
    cout << "\tMis-prediction rate\t\t\t\t\t\t: " << misPredictionRate << endl;
    cout << "============================================================" << endl;
}

void Predictor::findIfUnique(int branch) {
    if (branches.size() == 0)
        branches.push_back(branch);

    if (find(branches.begin(), branches.end(), branch) == branches.end())
        branches.push_back(branch);
}
