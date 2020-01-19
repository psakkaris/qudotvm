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

std::pair<bool, size_t> ThanosUnit::canSnap(QuWorld* quworld1, QuWorld* quworld2) {
    std::string qudna1 = getQuDna(quworld1);
    std::string qudna2 = getQuDna(quworld2);

    return canSnap(qudna1, qudna2);
}

std::pair<bool, size_t> ThanosUnit::canSnap(const std::string& qudna1, const std::string&  qudna2) const {
    std::cout << qudna1 << "\n" << qudna2 << "\n\n\n";
    auto diff = std::make_pair(false, 0);
    int num_deltas = 0;
    for (size_t i=0; i < qudna1.length(); i++) {
        if (qudna1[i] != qudna2[i]) {
            // only merge X vs Y diffs
            if (qudna1[i] == 'Z' || qudna2[i] == 'Z') return std::make_pair(false, 0);;

            num_deltas++;
            if (num_deltas > 1) {
                return std::make_pair(false, 0);
            }
            diff = std::make_pair(true, i+1);
        }
    }
    return diff;
}
}