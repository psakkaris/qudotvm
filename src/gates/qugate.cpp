#include "qudot/gates/qugate.h"

namespace qudot {

void QuGate::applyGate(QuMvN* qumvn, int q) {
    for (size_t i=0; i < qumvn->size(); i++) {
        applyGate(qumvn->getQuWorld(i), q);
    }
}

}