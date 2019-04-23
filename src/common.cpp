#include "qudot/common.h"

namespace qudot {
    bool isNotZero(const QuAmp& amp) {
        return fabs(amp.real()) > qudot::TOLERANCE || fabs(amp.imag()) > qudot::TOLERANCE;
    }
} // namespace qudot