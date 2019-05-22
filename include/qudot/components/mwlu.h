#ifndef __QUDOT_MWLU_H
#define __QUDOT_MWLU_H

#include "qudot/quworld.h"
#include "qudot/qumvn.h"
#include "qudot/intrinsics/fulladder.h"
#include "qudot/intrinsics/comparator.h"

namespace qudot {

class ManyWorldsLogicUnit {
    private:
        FullAdder full_adder;
        Comparator comparator;

        void addModN(QuWorld* quworld, const int val, const int N, const int start_q, const int end_q);

    public:
        void addModN(QuMvN* qumvn, const int val, const int N);
        void addModN(QuMvN* qumvn, const int val, const int N, const int start_q, const int end_q);
};

}

#endif