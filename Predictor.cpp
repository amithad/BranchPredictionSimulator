//
// Created by amitha on 2/13/17.
//

#include "Predictor.h"
#include <iostream>
#include <algorithm>

Predictor::Predictor() {
    for (int i = 0; i < BHT_SIZE; i++) {
        BHT.insert(pair<int, int>(i, 0));
    }
    for (int i = 0; i < BHT_TWO_BIT_SIZE; i++) {
        BHT2Bit.insert(pair<int, int>(i, 0));
    }
    globalHistorySize = (int) pow(2.0, BHT_CORRELATED_BITS);
    GBH = 0;
    for (int i = 0; i < globalHistorySize; i++) {
        map<int, int> *temp = new map<int, int>(); //handle leak later
        for (int j = 0; j < BHT_TWO_BY_TWO_SIZE; j++) {
            temp->insert(pair<int, int>(j, 0));
        }
        BHT2By2.push_back(*temp);
    }
    customGHSize = (int) pow(2.0, BHT_CUSTOM_GBH_BITS);
    GBH = 0;
    for (int i = 0; i < customGHSize; i++) {
        map<int, int> *temp = new map<int, int>(); //handle leak later
        for (int j = 0; j < BHT_CUSTOM_SIZE; j++) {
            temp->insert(pair<int, int>(j, 0));
        }
        BHTCustom.push_back(*temp);
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
    totalBranches++;
    findIfUnique(branch); //stat collector call

    int pos = branch % BHT_TWO_BY_TWO_SIZE;
    unsigned int GBHOld = GBH;
    updateGBH(value == 1); //update the global branch history register

    int result = BHT2By2.at(GBHOld).at(pos);
    if (result == 0) { //state: 00
        if (value == 1) {
            BHT2By2.at(GBHOld)[pos] = 1;
            incorrectPredictions++;
        } else
            correctPredictions++;
        return false;
    }
    if (result == 1) { //state: 01
        if (value == 1) {
            BHT2By2.at(GBHOld)[pos] = 3;
            incorrectPredictions++;
        } else {
            BHT2By2.at(GBHOld)[pos] = 0;
            correctPredictions++;
        }
        return false;
    }
    if (result == 2) { //state: 10
        if (value == 1) {
            BHT2By2.at(GBHOld)[pos] = 3;
            correctPredictions++;
        } else {
            BHT2By2.at(GBHOld)[pos] = 0;
            incorrectPredictions++;
        }
        return true;
    }
    if (result == 3) { //state: 11
        if (value == 1) {
            correctPredictions++;
        } else {
            BHT2By2.at(GBHOld)[pos] = 2;
            incorrectPredictions++;
        }
        return true;
    }
}

bool Predictor::BHTOptimized(int branch, int value) {
    totalBranches++;
    findIfUnique(branch); //stat collector call

    int pos = branch % BHT_CUSTOM_SIZE;
    unsigned int GBHOld = customGBH;
    updateCustomGBH(value == 1); //update the global branch history register

    int result = BHTCustom.at(GBHOld).at(pos);
    if (result == 0) { //state: 00
        if (value == 1) {
            BHTCustom.at(GBHOld)[pos] = 1;
            incorrectPredictions++;
        } else
            correctPredictions++;
        return false;
    }
    if (result == 1) { //state: 01
        if (value == 1) {
            BHTCustom.at(GBHOld)[pos] = 3;
            incorrectPredictions++;
        } else {
            BHTCustom.at(GBHOld)[pos] = 0;
            correctPredictions++;
        }
        return false;
    }
    if (result == 2) { //state: 10
        if (value == 1) {
            BHTCustom.at(GBHOld)[pos] = 3;
            correctPredictions++;
        } else {
            BHTCustom.at(GBHOld)[pos] = 0;
            incorrectPredictions++;
        }
        return true;
    }
    if (result == 3) { //state: 11
        if (value == 1) {
            correctPredictions++;
        } else {
            BHTCustom.at(GBHOld)[pos] = 2;
            incorrectPredictions++;
        }
        return true;
    }
}

void Predictor::updateGBH(bool branchTaken) { //bit shift
    int LSB = 0;
    branchTaken ? LSB = 1 : LSB = 0;
    GBH <<= 1; //shift bits
    GBH += LSB; //add latest entry
    GBH &= (globalHistorySize - 1); // bitwise and to ignore the msb
}

void Predictor::updateCustomGBH(bool branchTaken) { //bit shift
    int LSB = 0;
    branchTaken ? LSB = 1 : LSB = 0;
    customGBH <<= 1; //shift bits
    customGBH += LSB; //add latest entry
    customGBH &= (customGHSize - 1); // bitwise and to ignore the msb
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
