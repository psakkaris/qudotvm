#include "qudot/gates/H.h"

#include <string>

#include "qudot/common.h"
#include "qudot/gates/qugate.h" 
#include "qudot/quworld.h"

namespace qudot {

    void H::applyGate(QuWorld* qw, const int q) {
        int parents = qw->getRow(q);
        QuAmp p00 = qw->qudot_net[parents + 0];
        QuAmp p01 = qw->qudot_net[parents + 1];
        QuAmp p10 = qw->qudot_net[parents + 2];
        QuAmp p11 = qw->qudot_net[parents + 3];

        qw->qudot_net[parents + 0] = (p00 * ROOT2) + (p01 * ROOT2);
        qw->qudot_net[parents + 1] = (p00 * ROOT2) + (p01 * MROOT2);
        qw->qudot_net[parents + 2] = (p10 * ROOT2) + (p11 * ROOT2);
        qw->qudot_net[parents + 3] = (p10 * ROOT2) + (p11 * MROOT2);

        if (q < qw->num_qubits) {
            int children = qw->getRow(q+1);
            bool zeroActive = qw->isActive(q, ZERO);
            bool oneActive = qw->isActive(q, ONE);

            if (zeroActive && oneActive) {
                copyChildren(qw->qudot_net, children + 0, children + 2);
                copyChildren(qw->qudot_net, children + 1, children + 3);
            } else {
                if (!zeroActive) {
                    qw->deactivateChildren(q, ZERO);
                }
                if (!oneActive) {
                    qw->deactivateChildren(q, ONE);
                }
            }
        }
    }

    std::string H::getId() const {
        return ID;
    }

    void H::copyChildren(QuAmp* amps, const int c1, const int c2) {
        QuAmp a = amps[c1];
        QuAmp b = amps[c2];
        if (isNotZero(a) && !isNotZero(b)) {
            amps[c2] = QuAmp(a.real(), a.imag());
        }

        if (!isNotZero(a) && isNotZero(b)) {
            amps[c1] = QuAmp(b.real(), b.imag());
        }
    }
}