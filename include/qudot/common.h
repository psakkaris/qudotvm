#ifndef __QUDOT_QUCOMMON_H
#define __QUDOT_QUCOMMON_H

#include <complex>
#include <limits>
#include <cmath>

namespace qudot {
    typedef std::complex<float> QuAmp;
    typedef std::complex<double> QuAmp64;
    enum Qubit {ZERO, ONE};
    const unsigned char qu_stride=4;
    const float TOLERANCE = std::numeric_limits<float>::epsilon();
    const double TOLERANCE64 = std::numeric_limits<double>::epsilon();
    const QuAmp ZERO_AMP = QuAmp(0,0);
    const QuAmp ONE_AMP = QuAmp(1,0);
    const QuAmp MONE_AMP = QuAmp(-1, 0);
    const QuAmp I_AMP = QuAmp(0,1);
    const QuAmp MI_AMP = QuAmp(0,-1);
    const QuAmp ROOT2 = QuAmp(1.0 / std::sqrt(2), 0);
    const QuAmp MROOT2 = QuAmp(-1.0 / std::sqrt(2));

    const QuAmp64 ZERO_AMP64 = QuAmp64(0,0);
    const QuAmp64 ONE_AMP64 = QuAmp64(1,0);
    const char QUBIT_LABEL[2] = {'0', '1'}; 

    bool isNotZero(const QuAmp& amp);
}

#endif