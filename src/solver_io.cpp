#include <iostream>
#include "solver.h"

using std::vector;
using std::cout;
using std::endl;

void Solver::readVerilog(string fileName) {
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


void Solver::writeVerilog(string fileName) {
    /***********************************************************************
        Date:       2019.5.16
        Author:     Mr. Tsai
        Entry site: main.cpp - int main()
        Synopsis:   Writes the circuit to Verilog file.
    ***********************************************************************/

}
