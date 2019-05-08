#include "qudot/gates/X.h"

#include <string>

#include "qudot/gates/qugate.h"
#include "qudot/quworld.h"

namespace qudot {
    void X::applyGate(QuWorld* qw, const int q) {
        int parents = qw->getRow(q);
        // parent swap
        swapAmps(qw->qudot_net, parents + 0, parents + 1);
        swapAmps(qw->qudot_net, parents + 2, parents + 3);

        // swap children
        if (q < qw->num_qubits) {
            int children = qw->getRow(q+1);
            swapAmps(qw->qudot_net, children + 0, children + 2);
            swapAmps(qw->qudot_net, children + 1, children + 3);
        }
    }

    std::string X::getId() const {
        return ID;
    }  

    void X::swapAmps(QuAmp* amps, const int index1, const int index2) {
        QuAmp64 tmp = amps[index1];
        amps[index1] = amps[index2];
        amps[index2] = tmp;
    }
}