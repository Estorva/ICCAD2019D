#include <iostream>
#include "solver.h"

using std::vector;
using std::cout;
using std::endl;

Solver::Solver() {
    /***********************************************************************
        Date:       2019.5.20
        Author:     Mr. Pigeon
        Entry site: main.cpp - int main()
        Synopsis:   Constructor of Solver class. Clear vectors and fetch memory
                    for pointers _const0 and _const1;
    ***********************************************************************/
    _f.clear();
    _b.clear();

    _const0 = newGate(Gate::LOW);
    _const1 = newGate(Gate::HIGH);

    return;
}

void Solver::initial() {
    /***********************************************************************
        Date:       2019.5.20
        Author:     Mr. Pigeon
        Entry site: main.cpp - int main()
        Synopsis:   Initial the solver with a circuit for deubgging and testing.
    ***********************************************************************/

    // Gate declaration
    auto pi1 = newGate(Gate::PI, "pi 1");
    auto pi2 = newGate(Gate::PI, "pi 2");
    auto xg = newGate(Gate::XOR, "xor");
    auto inv = newGate(Gate::INV, "inv");
    auto po1 = newGate(Gate::PO, "po 1");

    auto pi3 = newGate(Gate::PI, "pi 3");
    auto pi4 = newGate(Gate::PI, "pi 4");
    auto nog = newGate(Gate::NOR, "nor");
    auto po2 = newGate(Gate::PO, "po 2");

    // Gate connection
    pair(pi1, xg);
    pair(pi2, xg);
    pair(xg, inv);
    pair(inv, po1);

    pair(pi3, nog);
    pair(pi4, nog);
    pair(nog, po2);

    return;
}

void Solver::report() {
    /***********************************************************************
        Date:       2019.5.20
        Author:     Mr. Pigeon
        Entry site: main.cpp - int main()
        Synopsis:   Write the statistics to the screen
    ***********************************************************************/
    cout << "\
============================= Circuit statistics ==============================="\
         << endl;
    cout << "Forward:" << endl;
    _f.report();
    cout << endl << "Backward:" << endl;
    _b.report();

    return;
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

    // Call convertToNand() two times
    convertToNand();
    convertToNand();
    // computeHash();
    // compare();
    // replace();
    convertToUni();

    return;
}


void Solver::convertToNand() {
    /***********************************************************************
        Date:       2019.5.20
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

    size_t size = _f.size();
    for (size_t i = 0; i < size; i++) {
        // for (auto it = _f.vBegin(); it != _f.vEnd(); it++) {
        // adding or removing elements invalidates the iterators
        // iterate through the list
        Gate* target = _f[i];
        AdjacencyList<Gate*>::Iterator jt;

        switch(target->type) {
            case Gate::INV:
            target->type = Gate::NAND;
            // Insert what is already fanin of INV again
            jt = _b.find(target);
            pair(*(++jt), target);
            break;


            case Gate::AND:
            case Gate::NOR: {
            // Add INV at output, and change target to OR/NAND

            if (target->type == Gate::AND)
                target->type = Gate::NAND;
            else // target->type == Gate::NOR
                target->type = Gate::OR;

            // Create a new INV gate
            auto newinv = newGate(Gate::INV);

            jt = _f.find(target);
            jt++;
            while (jt != _f.end()) {
                // Connect all fanouts of this gate to new INV
                _b.removeUnder(target, *jt);
                pair(newinv, *jt);
                jt++;
            }

            // Make target connect to INV only
            _f.removeNeighbor(target);
            pair(target, newinv);
            break;
            }


            case Gate::OR: {
            target->type = Gate::NAND;

            // Create INV's for each of OR's input
            auto newinv1 = newGate(Gate::INV);
            auto newinv2 = newGate(Gate::INV);

            jt = _b.find(target);
            jt++; // jt is now fanin1 of target
            // Make INV1 fanout of fanin1 of target
            _f.removeUnder(target, *jt);
            pair(*jt, newinv1);
            jt++;
            // Make INV2 fanout of fanin2 of target
            _f.removeUnder(target, *jt);
            pair(*jt, newinv2);

            // Pair INV1 and INV2 and target
            _b.removeNeighbor(target);
            pair(newinv1, target);
            pair(newinv2, target);
            break;
            }


            case Gate::XOR: {
            // type -> NAND
            // create 3 new NAND's
            // XOR(x, y) = NAND(NAND1(A, C), NAND2(C, B)) whr C = NAND3(A, B)
            target->type = Gate::NAND;
            auto newnand1 = newGate(Gate::NAND);
            auto newnand2 = newGate(Gate::NAND);
            auto newnand3 = newGate(Gate::NAND);

            // Separate fanins from XOR gate
            jt = _b.find(target);
            jt++;
            auto fanin1 = *jt; // fanin #1 of XOR
            jt++;
            auto fanin2 = *jt;
            _b.removeNeighbor(target);
            _f.removeUnder(target, fanin1);
            _f.removeUnder(target, fanin2);

            // Pair NAND1 and NAND2 w/ NAND(XOR)
            pair(newnand1, target);
            pair(newnand2, target);
            /*
            Code that is replaced by pair(...)
            _b.insertUnder(newnand1, target).insertUnder(newnand2, target);
            _f.insertUnder(target, newnand1).insertUnder(target, newnand2);
            */

            // Pair NAND1 w/ fanin1 and NAND3
            pair(fanin1, newnand1);
            pair(newnand3, newnand1);

            // Pair NAND2 w/ fanin2 and NAND3
            pair(fanin2, newnand2);
            pair(newnand3, newnand2);

            // Pair NAND3 w/ fanin1 and fanin2
            pair(fanin1, newnand3);
            pair(fanin2, newnand3);
            break;
            } // close the case to avoid "jump to case label" warning


            default:
            // handle other gates: keep as is
            break;
        }
    }

    return;
}

void Solver::convertToUni() {
    /***********************************************************************
        Date:       2019.5.20
        Author:     Mr. Pigeon
        Entry site: solver.cpp - void optimize()
        Synopsis:   NAND(x, y) = U1(.S0(x), .S1(y), .I0(0), .I1(0),
                                    .I2(0), ,I3(1))
    ***********************************************************************/
    size_t size = _f.size();
    for (size_t i = 0; i < size; i++) {
        Gate* target = _f[i];

        if (target->type == Gate::NAND) {
            target->type == Gate::U1;
            pair(_const0, target); // I0
            pair(_const0, target); // I1
            pair(_const0, target); // I2
            pair(_const1, target); // I3
        }
    }

    return;
}


Gate* Solver::newGate(Gate::Type type, string name) {
    /***********************************************************************
        Date:       2019.5.20
        Author:     Mr. Pigeon
        Entry site: solver.cpp - void Solver()
                                 void convertToNand()
                                 void convertToUni()
        Synopsis:   handles index of gates, registers new gate into _f and _b
    ***********************************************************************/
    size_t id;
    if (!_available_id.empty()) {
        id = _available_id.back();
        _available_id.pop_back();
    }
    else
        id = _f.size();

    Gate* newgate = new Gate(id, type, name);
    _f.insert(newgate);
    _b.insert(newgate);
    return newgate;
}


void Solver::pair(Gate* child, Gate* parent) {
    /***********************************************************************
        Date:       2019.5.20
        Author:     Mr. Pigeon
        Entry site: various
        Synopsis:   child -> parent
    ***********************************************************************/
    _f.insertUnder(parent, child);
    _b.insertUnder(child, parent);
    return;
}
