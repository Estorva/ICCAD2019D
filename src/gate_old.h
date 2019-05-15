#ifndef _GATE_H_
#define _GATE_H_

#include <cstring>

class Gate {
public:
    Gate();
    ~Gate();

    enum      Type { NA = 0,
                     PI,    PO,
                     PPI,   PPO,   PPI_IN, PPO_IN,
                     INV,   BUF,
                     AND2,  AND3,  AND4,
                     NAND2, NAND3, NAND4,
                     OR2,   OR3,   OR4,
                     NOR2,  NOR3,  NOR4,
                     XOR2,  XOR3,
                     XNOR2, XNOR3,
                     MUX,
                     TIE0,   TIE1, TIEX,   TIEZ
                   };

    // basic info
    int       id_;    // position in circuit gate array
    Type      type_;  // type of the gate
};

inline Gate::Gate() {
    id_      = -1;
    cid_     = -1;
    pmtid_   = 0;
    lvl_     = -1;
    frame_   = 0;
    type_    = NA;
    nfi_     = 0;
    fis_     = NULL;
    nfo_     = 0;
    fos_     = NULL;
    v_       = X;
    gl_      = PARA_L;
    gh_      = PARA_L;
    fl_      = PARA_L;
    fh_      = PARA_L;
    hasCons_ = false;
    cons_    = PARA_L;

    cc0_     = 0;
    cc1_     = 0;
    co_      = 0;

    depthFromPo_ = -1;
    fiMinLvl_    = -1;
}

inline Gate::~Gate() {}
