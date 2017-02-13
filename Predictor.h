//
// Created by amitha on 2/13/17.
//

#ifndef BHTPERF_PREDICTOR_H
#define BHTPERF_PREDICTOR_H

#include <map>
#include <vector>

using namespace std;

class Predictor {
public:
    Predictor();
    bool takeBranch(int PredictorID, int branch, int value); //called by Daemon to decide on branching
    void printStats(); //views the results

private:
    map<int, int> BHT;
    map<int, int> BHT2Bit;
    vector<int> branches;

    bool BHTOneBit(int branch, int value); //8192 BHT
    bool BHTTwoBit(int branch, int value); //2-bit 4096 BHT
    bool BHTTwoByTwo(int branch, int value); //2,2 1024 BHT
    bool BHTOptimized(int branch, int value); //Custom
    void findIfUnique(int branch);

    //====stats====================
    int totalBranches;
    int correctPredictions;
    int incorrectPredictions;
    //=============================

};

#endif //BHTPERF_PREDICTOR_H
