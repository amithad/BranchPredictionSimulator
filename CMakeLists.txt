cmake_minimum_required(VERSION 3.6)
project(BranchPredictionSimulator)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        Daemon.cpp
        Predictor.cpp
        Predictor.h)

add_executable(BPAnalysis ${SOURCE_FILES})