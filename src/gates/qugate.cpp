#include "qudot/gates/qugate.h"

#include "tbb/parallel_for.h"
#include "qudot/common.h"

namespace qudot {

void QuGate::applyGate(QuMvN* qumvn, int q) {
    tbb::parallel_for(qumvn->range(), [&](const WorldMap::range_type &r) {
        for (auto it = r.begin(); it != r.end(); ++it) {
            applyGate(qumvn->getQuWorld((*it).first), q);
        }
    });
    // for (auto it=qumvn->begin(); it != qumvn->end(); ++it) {
    //     applyGate(it->second.get(), q);
    // }
}

void QuGate::applyControlGate(QuMvN* qumvn, const int target, const std::vector<int>& ctrls) {
    qumvn->splitWorlds(ctrls);
    tbb::parallel_for(qumvn->range(), [&](const WorldMap::range_type &r) {
        for (auto it = r.begin(); it != r.end(); it++) {
            applyControlGate(qumvn->getQuWorld((*it).first), target, ctrls);
        }
    });    
    // for (auto it=qumvn->begin(); it != qumvn->end(); ++it) {
    //     applyControlGate(it->second.get(), target, ctrls);
    // }
}

void QuGate::applyControlGate(QuWorld* quworld, const int target, const std::vector<int>& ctrls) {
    if (quworld->areActive(ctrls, ONE)) {
        applyGate(quworld, target);
    }
}

}