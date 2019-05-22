#include "qudot/intrinsics/fulladder.h"

#include <vector>
#include "tbb/parallel_for.h"

namespace qudot {

void FullAdder::addClassicalInt(QuMvN* qumvn, const int value, const int start_q, const int end_q) {
    int num_input_q = qumvn->getNumQubits();
    int n = end_q - start_q + 1;
    int other_qubits = num_input_q - n;

    int q = 2*n + 1;

    std::vector<int> all_qubits;
    for (int i=1; i <= n; i++) {
        all_qubits.push_back(i);
    }
    qumvn->splitWorlds(all_qubits); 
    qumvn->setNumQubits(q+other_qubits);
    tbb::parallel_for(size_t(0), size_t(qumvn->size()), [&] (size_t i) {
        addClassicalInt(qumvn->getQuWorld(i), value, start_q, end_q);
    });
    qumvn->setNumQubits(num_input_q);

}

void FullAdder::addClassicalInt(QuWorld* quworld, const int value, const int start_q, const int end_q, const Qubit enabling_q) {
    if (quworld->getEnablingQubit(enabling_q)) {
        addClassicalInt(quworld, value, start_q, end_q);
    }
}

void FullAdder::addClassicalInt(QuWorld* quworld, const int value, const int start_q, const int end_q) {
    int num_input_q = quworld->getNumQubits();
    int n = end_q - start_q + 1;
    int other_qubits = num_input_q - n;

    int q = 2*n + 1;
    int num_new_q = (q+other_qubits) - num_input_q;
    int carry_q = 1;
    int other_above = start_q - 1;
    // Dimension of register below the end Qubit
    int other_below = num_input_q - end_q;
    // get QuRegisters ready for contraction at the end
    std::pair<int, int> qureg_above(n+2, (n+1) + other_above);
    std::pair<int, int> qureg_result(carry_q+1, carry_q+n);
    std::pair<int, int> qureg_below((q+other_qubits) - other_below + 1, (q+other_qubits));

    quworld->expandQubits(num_new_q);
    int val_copy = value;
    for (int i=0; i < n; i++) {
        int ri = carry_q + (n - i);
        int bi = ri + n + other_above;
        quworld->swapQubits(carry_q, ri);
        if ((val_copy & 1) == 1) {
            feynmanUnit.x.applyControlGate(quworld, carry_q, {ri});
            feynmanUnit.x.applyGate(quworld, ri);
        }
        feynmanUnit.x.applyControlGate(quworld, carry_q, {bi, ri});
        feynmanUnit.x.applyControlGate(quworld, ri, {bi});
        val_copy = val_copy >> 1;
    }

    quworld->contractQubits(num_input_q, qureg_above, qureg_result, qureg_below);
}

}