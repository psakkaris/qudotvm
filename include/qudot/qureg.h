#ifndef __QUDOT_QUREG_H
#define __QUDOT_QUREG_H

#include <vector>

/**
 * Represents a Qubit Register which supports a list of integers. This register can then be applied to gates.
 *
 * @since 0.1.0
 */
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
            void addQubit(const int qubit);

            int getStart() const;
            int getEnd() const;
            int getSize() const;    
    };
}

#endif