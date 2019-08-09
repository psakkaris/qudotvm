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

}