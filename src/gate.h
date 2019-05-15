#ifndef _GATE_H_
#define _GATE_H_

#include <string>

/*******************************************************************************
    Filename:   gate.h
    Date:       2019.04.26
    Author:     Mr. Pigeon
    Dependency: string.h
    Synopsis:   Definition of gates.

                Gates are an abstraction of assignments in verilog
                files. In principle, a "gate" can be any of ordinary gates
                we learned in our electronic design lessons. This definition
                of class "gate" also supports three universal gates, so that
                one can use this definition to deal with ICCAD contest question
                D. The following scheme shows the result of translation of
                a verilog sentence "k4 = (q1 & ~j) | (f4 & n)", taken from
                test1.v line 195.

                   +--+
                j -|i1|-+     +--+
                   +--+ +-----|i2|   +--+
                q1 -----------|& |---|  |
                              +--+   |k4|---...
                              +--+   |OR|
                f4 -----------|i3|---|  |
                n ------------|& |   +--+
                              +--+

                Although this is a single line of assignment, counting from
                the output literal k4 to input literal j, there are 3 operators,
                which means that this sentence translates into a three-level
                structure. Three intermediate (i1, i2, i3) gates are inserted;
                the number of intermediate gates is the number of operators in
                an assignment minus 1.

                In principle, a variable name is assigned to gates
                whose output node is named thereafter.
*******************************************************************************/

class Gate {
public:
    enum Type {
        NON = 0,
        AND,
        NAND,
        OR,
        NOR,
        XOR,
        XNOR,
        INV,
        U1,
        U2,
        U3,
        PI,
        PO
    };
    // U1: 4to1 MUX
    // U2: Actel’s ACT 1 logic module
    // U3: H-bridge function

    Gate(unsigned id, Type type, std::string name);
    ~Gate() {;}
    friend class Circuit;

    std::string report() const { return _name; }

private:
    unsigned int _id;
    std::string _name;
    Type _type;
};

inline Gate::Gate(unsigned id = 0,
                  Type type = NON,
                  std::string name = "") {
    _id = id;
    _type = type;
    _name = name;
}


#endif //_GATE_H_
