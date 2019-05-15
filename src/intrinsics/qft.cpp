#include "qudot/intrinsics/qft.h"
#include "qudot/components/feynmanunit.h"

namespace qudot {
    void Qft::operator() (QuMvN* qumvn, FeynmanUnit& feynmanUnit, const int start_qu, const int end_qu) {
        int _num_qubits = (end_qu - start_qu) + 1;
        for (int q = start_qu; q <= end_qu; q++) {
            feynmanUnit.h.QuGate::applyGate(qumvn, q);
            int i = q + 1;
            int r = 2;
            while (i <= end_qu) {
                feynmanUnit.getRGate(r)->applyControlGate(qumvn, i, {q});
                i += 1;
                r += 1;
            }
        }

        int midpoint = _num_qubits / 2;
        for (int i=0; i < midpoint; i++) {
            qumvn->swap(start_qu+i, end_qu-i);
        }
    }        
}
