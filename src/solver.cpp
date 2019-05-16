#include <iostream>
#include "solver.h"

using std::vector

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
    /***********************************************************************
        Date:       2019.5.15
        Author:     Mr. Pigeon
        Entry site: main.cpp - int main()
        Synopsis:   Convert gates into nands
                    Search the adjacency list to find candidates to convert
                    into universal gates
    ***********************************************************************/

    // Call convertToNand() two time
    convertToNand();
    convertToNand();
    convertToUni();
}

void Solver::writeVerilog() {
    /***********************************************************************
        Date:       2019.5.16
        Author:     Mr. Tsai
        Entry site: main.cpp - int main()
        Synopsis:   Writes the circuit to Verilog file.
    ***********************************************************************/

}


void Solver::convertToNand() {
    /***********************************************************************
        Date:       2019.5.15
        Author:     Mr. Pigeon
        Entry site: solver.cpp - void optimize()
        Synopsis:   All gates can be translated into NANDs:
                    INV(x) = NAND(x, x)
                    AND(x, y) = NAND(x, y) -> INV
                    OR(x, y) = NAND(x', y')
                    NOR(x, y) = NAND(x', y') -> INV
                    XOR(x, y) = NAND(NAND(A, C), NAND(C, B)) whr
                        C = NAND(A, B)
    ***********************************************************************/

    for (auto it = _f.vBegin(); it != _f.vEnd(); it++) {
        // iterate through the list
        Gate* target = ***it;

        switch(target->type) {
            case Gate::INV:
            target->type = Gate::NAND;
            // Insert what is already fanin of INV again
            auto jt = _b.find(target);
            _b.insertUnder(***(jt++), target);
            break;


            case Gate::AND:
            target->type = Gate::NAND;

            // Create a new INV gate
            auto *newinv = new Gate(9, Gate::INV);
            _b.insert(newinv);
            _f.insert(newinv);

            auto jt = _f.find(target);
            jt++;
            while (jt != _f.end()) {
                // Connect all fanouts of this gate to new INV
                _b.removeUnder(target, ***jt);
                _b.insertUnder(newinv, ***jt);

                _f.insertUnder(***jt, newinv);
                jt++;
            }

            // Make target connect to INV only
            _f.removeNeighbor(target);
            _f.insertUnder(newinv, target);

            // Make INV fanout of target
            _b.insertUnder(target, newinv);


            case Gate::OR:
            target->type = Gate::NAND;

            // Create INV's for each of OR's input
            auto *newinv1 = new Gate(9, Gate::INV);
            auto *newinv2 = new Gate(9, Gate::INV);
            _b.insert(newinv1).insert(newinv2);
            _f.insert(newinv1).insert(newinv2);

            jt = _b.find(target);
            jt++;
            // Make INV1 fanout of fanin1 of target
            _f.removeUnder(target, ***jt);
            _f.insertUnder(newinv1, ***jt);
            jt++;
            // Make INV2 fanout of fanin2 of target
            _f.removeUnder(target, ***jt);
            _f.insertUnder(newinv2, ***jt);

            // Pair INV1 and INV2 and target
            _b.removeNeighbor(target);
            _f.insertUnder(target, newinv1);
            _f.insertUnder(target, newinv2);
            _b.insertUnder(newinv1, target);
            _b.insertUnder(newinv2, target);


            case Gate::NOR:
            // Add INV at output, and change target to OR


            case Gate::XOR:
            // type -> NAND
            // create 3 new NAND's
            // XOR(x, y) = NAND(NAND(A, C), NAND(C, B)) whr C = NAND(A, B)
        }
    }
}
