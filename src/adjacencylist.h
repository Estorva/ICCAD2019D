#ifndef _ADJACENCYLIST_H_
#define _ADJACENCYLIST_H_

#include <vector>
#include <iostream>
#include "gate.h"

/*******************************************************************************
    Filename:   adjacencylist.h
    Date:       2019.04.26
    Author:     Mr. Pigeon
    Dependency: vector.h gate.h
    Synopsis:   Definition of adjacency list data structure.
                The connection of gates are represented by two adjacency lists:
                forward list and backward list. This removes the need to record
                relationships between gates from each gate instantiation, which
                could cause a great load considering there are a number of
                vectors. For example, given a circuit:

                        +--+
                1 -*----||3|-\
                    \ /-+--+  \-+--+ /------- 7
                     x          |^5|-*
                    / \-+--+  /-+--+ \  +--+
                2 -*----|&4|-/        \-|~6|- 8
                        +--+            +--+

                Forward list  Backward list
                1 -> 3 -> 4   3 -> 1 -> 2
                2 -> 3 -> 4   4 -> 1 -> 2
                3 -> 5        5 -> 3 -> 4
                4 -> 5        6 -> 5
                5 -> 6 -> 7   7 -> 5
                6 -> 8        8 -> 6
*******************************************************************************/

template <class T> class AdjacencyList;

template <class T>
class AdjacencyNode {
public:
    AdjacencyNode(T t = T(), AdjacencyNode<T>* next = 0) { _t = t; _next = next; }
    ~AdjacencyNode() {;}
    friend AdjacencyList<T>;

    const T& operator* () const { return _t; }
    T        operator* ()       { return _t; }

private:
    bool operator== (AdjacencyNode<T> n) const { return _t == n._t; }

    T _t;
    AdjacencyNode<T>* _next;
};

template <class T>
class AdjacencyList {
public:
    AdjacencyList() {
        _v.clear();
    }
    ~AdjacencyList() {;}

    class Iterator {
        // Iterates through a chain of adjacent vertices
        // Usage:   Iterator it = list.find(a);
        //          for (; it != list.end(); it++)

                  Iterator(AdjacencyNode<T>* n = 0) : _n(n) {;}
                  Iterator(const Iterator& i) : _n(i._n) {;}
                  ~Iterator() {;}

        friend    AdjacencyList<T>;

        const T&  operator*  () const { return **_n; } // One asterisk returns
        T         operator*  ()       { return **_n; } // the AdjacencyNode obj
        Iterator& operator++ ()       {
                  _n = (_n->_next == NULL ? _e : _n->_next);
                  return *this;
        }
        Iterator  operator++ (int)    {
                  // it++
                  Iterator temp = *this;
                  _n = (_n->_next == NULL ? _e : _n->_next);
                  return temp;
        }
        // -- not supported
        bool      operator== (const Iterator& it) const {
                  return this->_n == it._n;
        }
        bool      operator!= (const Iterator& it) const {
                  return !(*this == it);
        }

    private:
        AdjacencyNode<T>* _n;        // the node this iterator points to
        static AdjacencyNode<T>* _e;
        // end node when this iterator iterates through a list
    };


    bool              empty() const { return _v.size() <= 1; }
    Iterator          end() const   { return Iterator(Iterator::_e); }
    Iterator          find(T) const;
    bool              isIn(T) const;
    bool              isUnder(T, T) const;
    AdjacencyList<T>& insert(T);
    AdjacencyList<T>& insertUnder(T, T);
    AdjacencyList<T>& remove(T);
    AdjacencyList<T>& removeNeighbor(T);
    AdjacencyList<T>& removeUnder(T, T);
    void              report() const;
    size_t            size() const  { return _v.size(); }

    typename std::vector<T>::iterator vBegin() { return _v.begin(); }
    typename std::vector<T>::iterator vEnd() { return _v.end(); }


private:
    std::vector<AdjacencyNode<T>*> _v; // the vector that stores all pointers to nodes
    static AdjacencyNode<T>*  _e; // the end node that is not in the vector
};

// Static end node
template <class T>
AdjacencyNode<T>* AdjacencyList<T>::_e = new AdjacencyNode<Gate*>();
template <class T>
AdjacencyNode<T>* AdjacencyList<T>::Iterator::_e = AdjacencyList<T>::_e;


//========================= METHOD IMPLEMENTATION ============================//

// If a is AdjacencyNode<T>*:
// use **a to retrieve T
// if a is an iterator (AdjacencyList::Iterator or vector::Iterator)
// use ***a to retrieve T

template<class T>
inline typename AdjacencyList<T>::Iterator AdjacencyList<T>::find(T t) const {
    // Find T in the list
    for (auto pan : _v) {
        // pan: pointer of AdjacencyNode
        if (t == **pan) return Iterator(pan);
    }
    // t not found; return .end()
    return Iterator(_e);
}

template<class T>
inline bool AdjacencyList<T>::isIn(T t) const {
    // Find T in the vector _v
    for (auto pan : _v) {
        // pan: pointer of AdjacencyNode
        if (t == ***pan) return true;
    }
    // t not found; return .end()
    return false;
}

template<class T>
inline bool AdjacencyList<T>::isUnder(T u, T t) const {
    // See if u is in the list whose head is t
    for (Iterator it = find(t); it != end(); it++) {
        if (***it == u) return true;
    }
    return false;
}

template<class T>
inline AdjacencyList<T>& AdjacencyList<T>::insert(T t) {
    // Insert t into adjacency list
    auto *n = new AdjacencyNode<T>(t);

    for (auto it = _v.begin(); it != _v.end(); it++) {
        if (**it == *n) return *this;
        // Dont insert if it is already in the vector
    }
    _v.push_back(n);
    return *this;
}

template<class T>
inline AdjacencyList<T>& AdjacencyList<T>::insertUnder(T u, T t) {
    // Insert u under t
    // If t is absent: create one
    auto it = find(t);
    auto *m = new AdjacencyNode<T>(u);

    if (it == end()) {
        // Create a new node whose next is u, push it into the vector
        auto *n = new AdjacencyNode<T>(t, m);
        _v.push_back(n);
    }
    else {
        while (it._n->_next != NULL) it++;
        // make the last node in this list point at m
        it._n->_next = m;
    }
    return *this;
}

template<class T>
inline AdjacencyList<T>& AdjacencyList<T>::remove(T t) {
    // Remove t from _v
    // Doing so, you are supposed to know what you are doing
    for (auto it = _v.begin(); it != _v.end(); it++) {
        if (***it == t) {
            _v.erase(it);
            break;
        }
    }
    return *this;
}

template<class T>
inline AdjacencyList<T>& AdjacencyList<T>::removeNeighbor(T t) {
    // Remove all adjacency vertices of this vertex
    auto it = find(t);
    std::vector<AdjacencyNode<T>*> gv; // garbage vector
    gv.clear();

    if (it == end()) return *this; // t not found

    while (it._n->_next != NULL) {
        it++;
        gv.push_back(it._n);
    }

    while(!gv.empty()) {
        delete gv.back();
        gv.pop_back();
    }

    it = find(t);
    it._n->_next = NULL;

    return *this;
}

template<class T>
inline AdjacencyList<T>& AdjacencyList<T>::removeUnder(T u, T t) {
    // Remove u from the list whose head is t
    // If there are multiple instances of u under t, use isUnder to check
    auto it1 = find(t);
    auto it2 = it1;
    AdjacencyNode<T>* garbage = 0;

    if (it1 == end()) return *this; // t not found

    while (it1._n->_next != NULL) {
        it1++; // it1 leads it2 by 1 node
        if (*it1 == u) {
            garbage = it1._n;
            it2._n->_next = it1._n->_next;
            break;
        }
        it2++;
    }

    if (garbage) delete garbage;
    return *this;
}

template<class T>
inline void AdjacencyList<T>::report() const {
    for (auto i = _v.begin(); i != _v.end(); i++) {
        std::cout << "Gate " << (***i)->name << " is connected to:" << std::endl;
        std::cout << "    ";
        Iterator j(*i); // j points to what i points to
        j++; // skip self
        for (; j != end(); j++) {
            std::cout << (*j)->name << " "; // calls report() of Gate
        }
        std::cout << std::endl;
    }
}


#endif //_ADJACENCYLIST_H_
