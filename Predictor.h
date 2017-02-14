//
// Created by amitha on 2/13/17.
//

#ifndef BHTPERF_PREDICTOR_H
#define BHTPERF_PREDICTOR_H

#include <map>
#include <vector>

#define BHT_SIZE 8192
#define BHT_TWO_BIT_SIZE 4096
#define BHT_TWO_BY_TWO_SIZE 1024
#define BHT_CORRELATED_BITS 2

//optimized
//NOTE: total BHT entries have been kept constant(8k)
//edit the following values to make optimizations to the history size and bht lengths
#define BHT_CUSTOM_SIZE 32
#define BHT_CUSTOM_GBH_BITS 7


using namespace std;

class Predictor {
public:
    Predictor();
    bool takeBranch(int PredictorID, int branch, int value); //called by Daemon to decide on branching
    void printStats(); //views the results

private:
    map<int, int> BHT; // 1 bit BHT
    map<int, int> BHT2Bit; // 2 bit BHTS
    vector<map <int,int>> BHT2By2; // 2,2 BHTS
    vector<int> branches;
    int globalHistorySize;
    unsigned int GBH; //global branch history

    //optimized
    vector<map <int,int>> BHTCustom; // Custom BHTS
    int customGHSize;
    unsigned int customGBH;

    bool BHTOneBit(int branch, int value); //8192 BHT
    bool BHTTwoBit(int branch, int value); //2-bit 4096 BHT
    bool BHTTwoByTwo(int branch, int value); //2,2 1024 BHT
    void findIfUnique(int branch);
    void updateGBH(bool branchTaken);

    //optimized
    void updateCustomGBH(bool branchTaken);
    bool BHTOptimized(int branch, int value); //Custom

    //====stats====================
    int totalBranches;
    int correctPredictions;
    int incorrectPredictions;
    //=============================

};

#endif //BHTPERF_PREDICTOR_H
