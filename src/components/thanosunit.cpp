#include "qudot/components/thanosunit.h"
#include "qudot/common.h"

namespace qudot {

QuWorld* ThanosUnit::createWorld(const size_t num_qubits, const size_t world_id, const size_t val) {
    QuWorld* quworld = new QuWorld(num_qubits, world_id, ONE_AMP64);
    size_t val_copy = val;

    for (int q=num_qubits; q >= 1; --q) {
        if ( (val_copy & 1) == 1) {
            feynmanUnit.x.applyGate(quworld, q);
        }
        val_copy = val_copy >> 1;
    }
    return quworld;
}    

std::string ThanosUnit::getQuDna(const QuWorld* quworld) const {
    char sig[quworld->getNumQubits()+1];
    for (int q=1; q <= quworld->getNumQubits(); q++) {
        bool zero_active = quworld->isActive(q, ZERO);
        bool one_active = quworld->isActive(q, ONE);

        if (zero_active && one_active) {
            sig[q-1]= 'Z';
        } else if (zero_active) {
            sig[q-1] = 'X';
        } else {
            sig[q-1] = 'Y';
        }
    }
    sig[quworld->getNumQubits()] = '\0';
    return std::string(sig);
}

}