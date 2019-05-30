#include "qudot/intrinsics/qftinv.h"
#include "qudot/components/eprunit.h"

#include <iostream>
namespace qudot {

    void QftInv::operator() (QuMvN* qumvn, FeynmanUnit& feynmanUnit, const int start_qu, const int end_qu) {
            int n = (end_qu - start_qu) + 1;;
            int midpoint = n / 2;

            for (int i=0; i < midpoint; i++) {
                qumvn->swap(start_qu+i, end_qu-i);
            }

            for (int q = end_qu; q >= start_qu ; q--) {
                std::cout << "IQFT: " << q << "\n";
                feynmanUnit.h.QuGate::applyGate(qumvn, q);

                int i = q - 1;
                int r = 2;
                while (i >= start_qu) {
                    feynmanUnit.getRDaggerGate(r)->applyControlGate(qumvn, i, {q});
                    i -= 1;
                    r += 1;
                }
                EPRUnit::mergeWorlds(qumvn);
            }
    }    

}