#ifndef __QUDOT_EPRUNIT_H
#define __QUDOT_EPRUNIT_H

#include "qudot/common.h"
#include "qudot/qumvn.h"

namespace qudot {
    namespace EPRUnit {
        void mergeWorlds(QuMvN* qumvn, double epsilon = TOLERANCE64);
    }    
}

#endif