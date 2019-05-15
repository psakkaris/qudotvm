#ifndef __QUDOT_QFT_H
#define __QUDOT_QFT_H

#include <string>
#include <unordered_map>

#include "qudot/gates/R.h"
#include "qudot/components/feynmanunit.h"
#include "qudot/qumvn.h"

namespace qudot {

class Qft {
    public:
        void operator() (QuMvN*, FeynmanUnit&, const int, const int);
};

}
#endif