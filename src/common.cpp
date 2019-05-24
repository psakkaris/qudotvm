#include "qudot/common.h"

namespace qudot {
    bool isNotZero(const QuAmp& amp) {
        return fabs(amp.real()) > qudot::TOLERANCE || fabs(amp.imag()) > qudot::TOLERANCE;
    }

    bool isNotZero(const QuAmp64& amp, double epsilon) {
        return fabs(amp.real()) > epsilon || fabs(amp.imag()) > epsilon;    
    }
} // namespace qudot