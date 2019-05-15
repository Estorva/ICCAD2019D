#include <iostream>
#include "solver.h"



void Solver::readVerilog(string filename) {
    /***********************************************************************
        Date:       2019.4.26
        Author:     Mr. Tsai
        Entry site: main.cpp - int main()
        Synopsis:   Opens file <filename>,
                    Parse each line into variables:
                        Create PI gates after the "input" keyword
                        Create PO gates after the "output" keyword
                        Ignore the section after "wire" keyword (or you can
                        count number of gates using this section)
                        For each assignment line, create (intermediate) gates
                        For each gate, initialize it w/ its name and an id
                     Connect gates (forward and backward)
    ***********************************************************************/
}

void Solver::optimize() {
    // TODO
}

void Solver::writeVerilog() {
    // TODO
}
