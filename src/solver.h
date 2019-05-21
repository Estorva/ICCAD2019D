#ifndef _SOLVER_H_
#define _SOLVER_H_

#include <iostream>
#include <string>
#include "adjacencylist.h"
#include "gate.h"

using std::string;
using std::vector;

/*******************************************************************************
    Filename:   solver.h
    Date:       2019.04.26
    Author:     Mr. Pigeon
    Dependency: adjacencylist.h
    Synopsis:   Definition of top module "solver", which is composed of several
                vital components in order to work and function properly. The
                components are:
                  - Reader: translates verilog file (variable names and
                    operators) into gate abstractions
                  - Optimizer: performs heuristics to find economic solution
                  - Writer: writes gates into verilog file.
*******************************************************************************/

class Solver {
public:
    Solver();
    ~Solver() {;}

    void initial(); // dummy debugging function
    void readVerilog(string fileName);
    void report();
    void optimize();
    void writeVerilog(string fileName);

private:
    void convertToNand();
    void convertToUni();
    void pair(Gate*, Gate*);
    Gate* newGate(Gate::Type, string = "");

    AdjacencyList<Gate*> _b; // Backward adjacency list
    AdjacencyList<Gate*> _f; // Forward adjacency list

    Gate* _const0;
    Gate* _const1;

    vector<size_t> _available_id;
};

#endif // _SOLVER_H_
