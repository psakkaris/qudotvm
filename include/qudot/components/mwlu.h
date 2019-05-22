#ifndef __QUDOT_MWLU_H
#define __QUDOT_MWLU_H

#include <vector>

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
        void ctrlAddModN(QuWorld* quworld, const int val, const int N, const int start_q, const int end_q, const std::vector<int>& ctrls);
        void mulModN(QuWorld* quworld, const int val, const int N, const int start_q, const int end_q);
        void ctrlMulModN(QuWorld* quworld, const int val, const int N, const int start_q, const int end_q, const std::vector<int>& ctrls);

    public:
        // addition methods
        void addModN(QuMvN* qumvn, const int val, const int N);
        void addModN(QuMvN* qumvn, const int val, const int N, const int start_q, const int end_q);
        void ctrlAddModN(QuMvN* qumvn, const int val, const int N, const int start_q, const int end_q, const std::vector<int>& ctrls);
        // multiplication methods
        void mulModN(QuMvN* qumvn, const int val, const int N);
        void mulModN(QuMvN* qumvn, const int val, const int N, const int start_q, const int end_q);
        void ctrlMulModN(QuMvN* qumvn, const int val, const int N, const int start_q, const int end_q, const std::vector<int>& ctrls);
};

}

#endif