#ifndef __QUDOT_COMPONENTS_FEYNMANUNIT_H
#define __QUDOT_COMPONENTS_FEYNMANUNIT_H

#include <memory>

#include "qudot/gates/qugate.h"
#include "qudot/gates/X.h"
#include "qudot/gates/H.h"
#include "qudot/gates/R.h"

namespace qudot {
    struct FeynmanUnit {
        X x;
        H h;
        R z;
        R s;
        R sdag;
        R t;
        R tdag;

        FeynmanUnit();
        std::unique_ptr<R> getRGate(int);
        std::unique_ptr<R> getRDaggerGate(int);
    };
}

#endif