#ifndef __QUDOT_GATES_QUGATE_H
#define __QUDOT_GATES_QUGATE_H

#include <string>

#include "qudot/quworld.h"

namespace qudot {
    class QuGate {
        public:
            virtual void applyGate(QuWorld*, const int) = 0;
            virtual std::string getId() const = 0;
    };
}
#endif