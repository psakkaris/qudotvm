#ifndef __QUDOT_FULLADDER_H
#define __QUDOT_FULLADDER_H

#include "qudot/common.h"
#include "qudot/qumvn.h"
#include "qudot/components/feynmanunit.h"

namespace qudot{

class FullAdder {
    private:
        FeynmanUnit feynmanUnit;
        
    public:
        void addClassicalInt(QuMvN* qumvn, const int value, const int start_q, const int end_q);
        void addClassicalInt(QuWorld* quworld, const int value, const int start_q, const int end_q);
        void addClassicalInt(QuWorld* quworld, const int value, const int start_q, const int end_q, const Qubit enabling_q);
};

}

#endif