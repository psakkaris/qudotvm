#ifndef __QUDOT_GATESTACKFRAME_H
#define __QUDOT_GATESTACKFRAME_H

#include <memory>

#include "qudot/bytecodes.h"
#include "qudot/qureg.h"

namespace qudot {
    class GateStackFrame {
        private:
            std::shared_ptr<GateAsmSymbol> symbol;
            int return_address;
            int* int_regs;
            QuReg* quregs;

        public:
            GateStackFrame(std::shared_ptr<GateAsmSymbol> asm_symbol, int);
            ~GateStackFrame();

            std::shared_ptr<GateAsmSymbol> getSymbol() const;
            int getReturnAddress() const;
            int* getIntRegs() const;
            QuReg* getQuRegs() const;
    };
}

#endif