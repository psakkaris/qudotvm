#ifndef __QUDOT_COMPARATOR_H
#define __QUDOT_COMPARATOR_H

#include <vector>

#include "qudot/quworld.h"
#include "qudot/qumvn.h"
#include "qudot/components/feynmanunit.h"

namespace qudot {

class Comparator {
    private:
        FeynmanUnit feynmanUnit;

    public:
        void compare(QuMvN* qumvn, const int val, const int start_q, const int end_q);
        void compare(QuMvN* qumvn, const int val, const int start_q, const int end_q, const std::vector<int>& ctrls);
        void compare(QuWorld* quworld, const int val, const int start_q, const int end_q);
};

};

#endif