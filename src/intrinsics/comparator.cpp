#include "qudot/intrinsics/comparator.h"

#include <utility>

//#include "tbb/parallel_for.h"

namespace qudot {

void Comparator::compare(QuMvN* qumvn, const int val, const int start_q, const int end_q) {
    int n = end_q - start_q + 1;
    int num_input_q = qumvn->getNumQubits();
    int other_q = num_input_q - n;
    int q = 2*n;

    std::vector<int> ctrls;
    for (int i=start_q; i <= end_q; i++) {
        ctrls.push_back(i);
    }
    qumvn->splitWorlds(ctrls);
    qumvn->setNumQubits((q+other_q) - num_input_q);

    // tbb::parallel_for(size_t(0), size_t(qumvn->size()), [&](size_t i) {
    //     compare(qumvn->getQuWorld(i), val, start_q, end_q);
    // });
    for (auto it=qumvn->begin(); it != qumvn->end(); ++it) {
        QuWorld* quworld = it->second.get();
        compare(quworld, val, start_q, end_q);
    }

    qumvn->setNumQubits(num_input_q);
}

void Comparator::compare(QuMvN* qumvn, const int val, const int start_q, const int end_q, const std::vector<int>& ctrls) {
    int n = end_q - start_q + 1;
    int num_input_q = qumvn->getNumQubits();
    int other_q = num_input_q - n;
    int q = 2*n;

    std::vector<int> all_ctrls(ctrls);
    for (int q=start_q; q <= end_q; q++) {
        all_ctrls.push_back(q);
    }
    qumvn->splitWorlds(all_ctrls);

    qumvn->setNumQubits((q+other_q) - num_input_q);

    // tbb::parallel_for(size_t(0), size_t(qumvn->size()), [&](size_t i) {
    //     QuWorld* quworld = qumvn->getQuWorld(i);
    //     if (quworld->areActive(ctrls, ONE)) {
    //         compare(quworld, val, start_q, end_q);
    //     }
    // });
    for (auto it=qumvn->begin(); it != qumvn->end(); ++it) {
         QuWorld* quworld = it->second.get();
        if (quworld->areActive(ctrls, ONE)) {
            compare(quworld, val, start_q, end_q);
        }       
    }

    qumvn->setNumQubits(num_input_q);
}

void Comparator::compare(QuWorld* quworld, const int val, const int start_q, const int end_q) {
    int n = end_q - start_q + 1;
    int num_input_q = quworld->getNumQubits();
    int other_q = num_input_q - n;
    int q = 2*n;

    // Dimension of register above the starting Qubit
    int other_above = start_q - 1;
    // Dimension of register below the end Qubit
    int other_below = num_input_q - end_q;

    int w = 1;
    int bi = other_above + n + 1;
    int gi = w + 1;
    int ai = n - 1;
    int b0 = bi + n - 1;
    quworld->expandQubits((q+other_q) - num_input_q);

    //first qubit
    if (((val >> ai) & 1) == 1) {
        feynmanUnit.x.applyControlGate(quworld, gi, {bi});
        feynmanUnit.x.applyGate(quworld, bi);
        feynmanUnit.x.applyControlGate(quworld, w, {bi});
    } else {
        feynmanUnit.x.applyGate(quworld, bi);
        feynmanUnit.x.applyControlGate(quworld, gi, {bi});
    }
    // middle qubits
    for (int i=1; i < n-1; i++) {
        int bii = bi + i;
        int gii = (gi-1) + i;
        int aii = ai - i;   
        if ( ((val >> aii) & 1) == 1 ) {
            feynmanUnit.x.applyControlGate(quworld, gii+1, {gii, bii});
            feynmanUnit.x.applyGate(quworld, bii);
            feynmanUnit.x.applyControlGate(quworld, w, {gii, bii});
        } else {
            feynmanUnit.x.applyGate(quworld, bii);
            feynmanUnit.x.applyControlGate(quworld, gii+1, {gii, bii});
        }     
    }
    // last qubit
    if ( (val & 1) == 1 ) {
        feynmanUnit.x.applyGate(quworld, b0);
        feynmanUnit.x.applyControlGate(quworld, w, {n, b0});
        // reverse the x gate
        feynmanUnit.x.applyGate(quworld, b0);
    }
    for (int i=bi; i < b0; i++) {
        feynmanUnit.x.applyGate(quworld, i);
    }
    quworld->setEnablingQubit(1);
    std::pair<int, int> qureg_above(n+1, n+other_above);
    std::pair<int, int> qureg_result(bi, bi+n-1);
    std::pair<int, int> qureg_below(quworld->getNumQubits() - other_below + 1, quworld->getNumQubits());

    quworld->contractQubits(num_input_q, qureg_above, qureg_result, qureg_below);
}

}