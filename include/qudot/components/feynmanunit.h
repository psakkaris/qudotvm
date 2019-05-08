#ifndef __QUDOT_COMPONENTS_FEYNMANUNIT_H
#define __QUDOT_COMPONENTS_FEYNMANUNIT_H

#include "qudot/gates/X.h"
#include "qudot/gates/H.h"

namespace qudot {
    struct FeynmanUnit {
        X x;
        H h;
    };
}

#endif