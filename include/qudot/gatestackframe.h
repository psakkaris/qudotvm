#ifndef __QUDOT_GATESTACKFRAME_H
#define __QUDOT_GATESTACKFRAME_H

#include <memory>

#include "qudot/bytecodes.h"
#include "qudot/qureg.h"

/**
 * GateStackFrame represents an Activation Record for making gate calls. It tracks the GateAsmSymbol, return address,
 * number of registers used by the gate and number of qubit registers used by the gate.
 *
 * @see GateAsmSymbol
 * @since 0.1.0
 */

namespace qudot {
    class GateStackFrame {
        private:
            std::shared_ptr<GateAsmSymbol> symbol;
            int return_address;
            std::vector<int> int_regs;
            std::vector<QuReg> quregs;

        public:
            GateStackFrame(std::shared_ptr<GateAsmSymbol> asm_symbol, int);

            std::shared_ptr<GateAsmSymbol> getSymbol() const;
            int getReturnAddress() const;
            const std::vector<int>& getIntRegs() const;
            const std::vector<QuReg>& getQuRegs() const;

            void setIntReg(const int, const int);
            void setQuReg(const int, const int);
            void addQuReg(const int, const int);
    };
}

#endif