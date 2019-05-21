#include "solver.h"

/*******************************************************************************
    Filename:   main.cpp
    Date:       2019.05.20
    Author:     Mr. Pigeon
    Dependency: adjacencylist.h solver.h
    Synopsis:   Compile me as main in "make". Use "make dbg" for debugging.
*******************************************************************************/

int main() {
    Solver solver;
    solver.initial();
    solver.report();
    solver.optimize();
    solver.report();
    return 0;
}
