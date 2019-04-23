#ifndef __QUDOT_QUREG_H
#define __QUDOT_QUREG_H

#include <vector>

namespace qudot {
    class QuReg {
        private:
            std::vector<int> qubits;
            int start;
            int end;

        public:
            QuReg();
            QuReg(int, int);

            const std::vector<int>& getQubits() const;

            int getStart() const;
            int getEnd() const;
            int getSize() const;    
    };
}

#endif