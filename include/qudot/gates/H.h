#ifndef __QUDOT_GATES_H_H
#define __QUDOT_GATES_h_H

#include <string>

#include "qudot/common.h"
#include "qudot/gates/qugate.h"
#include "qudot/quworld.h"

namespace qudot {
    class H : public QuGate {
        private:
            inline static const std::string ID = "H";
            void copyChildren(QuAmp*, const int, const int);

        public:
            void applyGate(QuWorld*, const int) override;        
            std::string getId() const override;
    };
}
#endif