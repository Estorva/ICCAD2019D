#include "adjacencylist.h"

/*******************************************************************************
    Filename:   test_adjacencylist.cpp
    Date:       2019.04.26
    Author:     Mr. Pigeon
    Dependency: adjacencylist.h
    Synopsis:   Compile me as main in "make al".
*******************************************************************************/

int main() {
    // Test adjacency list here

    /*********************** Gate declaration **************************/
    Gate pi1(0, Gate::PI, "pi 1");
    Gate pi2(1, Gate::PI, "pi 2");
    Gate ag(2, Gate::AND, "and");
    Gate og(3, Gate::OR, "or");
    Gate xg(4, Gate::XOR, "xor");
    Gate po(5, Gate::PO, "po");

    AdjacencyList<Gate*> al;
    /*********************** Gate registration **************************/
    al.insert(&pi1).insert(&pi2).insert(&ag).insert(&og).insert(&xg).insert(&po);

    /*********************** Gate connection **************************/
    al.insertUnder(&ag, &pi1).insertUnder(&ag, &pi2);
    al.insertUnder(&og, &pi1).insertUnder(&og, &pi2);
    al.insertUnder(&xg, &ag).insertUnder(&xg, &og);
    al.insertUnder(&po, &xg);

    al.remove(&ag);
    al.removeUnder(&po, &xg);
    al.removeUnder(&pi1, &xg); // not actually under that
    al.remove(&ag); // not in the list

    al.report();
}
