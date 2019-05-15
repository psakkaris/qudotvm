#ifndef __QUDOT_QFTINV_H
#define __QUDOT_QFTINV_H

#include <string>
#include <unordered_map>

#include "qudot/gates/R.h"
#include "qudot/components/feynmanunit.h"
#include "qudot/qumvn.h"

namespace qudot {

class QftInv {
    public:
        void operator() (QuMvN*, FeynmanUnit&, const int, const int);
};

}
#endif