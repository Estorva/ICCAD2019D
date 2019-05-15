#ifndef _CIRCUIT_H_
#define _CIRCUIT_H_

#include <string>
#include "gate.h"
#include "adjacencylist.h"

/*******************************************************************************
    Filename:   circuit.h
    Date:       2019.04.26
    Author:     Mr. Pigeon
    Dependency: gate.h adjacencylist.h
    Synopsis:   Definition of circuits.

                Circuits are a higher level of abstraction, and there is usually
                only one circuit in a solver at a moment. A circuit contains
                gates and necessary informations, and it provides an interface
                for optimizers to perform more smoothly.
*******************************************************************************/



class Circuit {
public:
                         Circuit();
                         ~Circuit() {;}

    // Arguments are filled in after implementation is finished in circuit.cpp
    void                 findGate();
    void                 replaceGate();
    void                 report();

private:
    *Gate                _pi;  // A pointer pointing to an array of PI
    *Gate                _po;
    int                  _numPI;
    int                  _numPO;
    AdjacencyList<*Gate> _forward;
    AdjacencyList<*Gate> _backward;
};

inline Circuit::Circuit() {
    _input = 0;
    _output = 0;
    _numPI = 0;
    _numPO = 0;
}



#endif //_CIRCUIT_H_
