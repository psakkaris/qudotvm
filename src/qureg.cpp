#include "qudot/qureg.h"

#include <vector>

namespace qudot {

    QuReg::QuReg() {}

    QuReg::QuReg(int _start, int _end) : start(_start), end(_end) {
        for (int i=start; i<=end; i++) {
            qubits.push_back(i);    
        }
    }

    const std::vector<int>& QuReg::getQubits() const{
        return qubits;
    }

    int QuReg::getStart() const {
        return start;
    }

    int QuReg::getEnd() const {
        return end;
    }

    int QuReg::getSize() const {
        return end - start + 1;
    }
}