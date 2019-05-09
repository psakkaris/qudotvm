#include "qudot/gates/Y.h"

#include <string>

#include "qudot/common.h"
#include "qudot/quworld.h"

namespace qudot {
    void Y::applyGate(QuWorld* qw, const int q) {
        int parents = qw->getRow(q);
        // parent swap
        swapAmps(qw->qudot_net, parents + 0, parents + 1);
        swapAmps(qw->qudot_net, parents + 2, parents + 3);

        // multiply phase
        qw->qudot_net[parents + 1] = qw->qudot_net[parents+1] * I_AMP;
        qw->qudot_net[parents + 3] = qw->qudot_net[parents+3] * I_AMP;
        qw->qudot_net[parents + 0] = qw->qudot_net[parents+0] * MI_AMP;
        qw->qudot_net[parents + 2] = qw->qudot_net[parents+2] * MI_AMP;

        // swap children
        if (q < qw->num_qubits) {
            int children = qw->getRow(q+1);
            swapAmps(qw->qudot_net, children + 0, children + 2);
            swapAmps(qw->qudot_net, children + 1, children + 3);
        }
    }

    std::string Y::getId() const {
        return ID;
    }  

    void Y::swapAmps(QuAmp* amps, const int index1, const int index2) {
        QuAmp64 tmp = amps[index1];
        amps[index1] = amps[index2];
        amps[index2] = tmp;
    }    
}