#include "common.h"

 bool isNotZero(QuAmp& amp) {
    return fabs(amp.real()) > TOLERANCE || fabs(amp.imag()) > TOLERANCE;
}