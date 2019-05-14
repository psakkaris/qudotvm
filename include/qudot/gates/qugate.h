#ifndef __QUDOT_GATES_QUGATE_H
#define __QUDOT_GATES_QUGATE_H

#include <string>

#include "qudot/quworld.h"
#include "qudot/qumvn.h"

namespace qudot {
    class QuGate {
        public:
            void applyGate(QuMvN*, int);
            virtual void applyGate(QuWorld*, const int) = 0;
            virtual std::string getId() const = 0;
    };
}
#endif