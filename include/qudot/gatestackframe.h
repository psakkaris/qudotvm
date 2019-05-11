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
            GateAsmSymbol* symbol;
            int return_address;
            int* int_regs;
            QuReg* quregs;

        public:
            GateStackFrame(GateAsmSymbol* asm_symbol, int);
            ~GateStackFrame();

            GateAsmSymbol* getSymbol() const;
            int getReturnAddress() const;
            int* getIntRegs() const;
            QuReg* getQuRegs() const;
    };
}

#endif