#ifndef __QUDOT_GATES_X_H
#define __QUDOT_GATES_X_H

#include <string>

#include "qudot/common.h"
#include "qudot/gates/qugate.h"
#include "qudot/quworld.h"

namespace qudot {
    class X : public QuGate {
        private:
            inline static const std::string ID = "X";
            void swapAmps(QuAmp*, const int, const int);

        public:
            void applyGate(QuWorld*, const int) override;        
            std::string getId() const override;
    };
}
#endif