#ifndef __QUDOT_GATES_R_H
#define  __QUDOT_GATES_R_H

#include <string>

#include "qudot/gates/qugate.h"

namespace qudot {
    class R : public QuGate {
        private:
            std::string _id;
            int _phase_k;
            QuAmp _phase;

        public:
            R(int phase_k, bool dagger = false);
            std::string getId() const override;
            void applyGate(QuWorld*, int) override;  

        friend inline bool operator==(const R& lhs, const R& rhs);  
        friend inline bool operator!=(const R& lhs, const R& rhs);    
    };
}
#endif