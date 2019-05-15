#ifndef __QUDOT_GATES_QUGATE_H
#define __QUDOT_GATES_QUGATE_H

#include <cstddef>
#include <string>
#include <vector>

#include "qudot/quworld.h"
#include "qudot/qumvn.h"

namespace qudot {
    class QuGate {
        public:
            void applyGate(QuMvN*, int);
            void applyControlGate(QuMvN*, const int, const std::vector<int>&);
            void applyControlGate(QuWorld*, const int, const std::vector<int>&);
            virtual void applyGate(QuWorld*, const int) = 0;
            virtual std::string getId() const = 0;
    };
}
#endif