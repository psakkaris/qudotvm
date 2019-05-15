#include "qudot/gates/qugate.h"

#include "qudot/common.h"

namespace qudot {

void QuGate::applyGate(QuMvN* qumvn, int q) {
    for (size_t i=0; i < qumvn->size(); i++) {
        applyGate(qumvn->getQuWorld(i), q);
    }
}

void QuGate::applyControlGate(QuMvN* qumvn, const int target, const std::vector<int>& ctrls) {
    qumvn->splitWorlds(ctrls);
    for (size_t i=0; i < qumvn->size(); i++) {
        applyControlGate(qumvn->getQuWorld(i), target, ctrls);
    }
}

void QuGate::applyControlGate(QuWorld* quworld, const int target, const std::vector<int>& ctrls) {
    if (quworld->areActive(ctrls, ONE)) {
        applyGate(quworld, target);
    }
}

}