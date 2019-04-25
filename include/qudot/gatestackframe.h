#ifndef __QUDOT_GATESTACKFRAME_H
#define __QUDOT_GATESTACKFRAME_H

#include "qudot/bytecodes.h"
#include "qudot/qureg.h"

namespace qudot {
    class GateStackFrame {
        private:
            GateAsmSymbol symbol;
            int return_address;
            int* int_regs;
            QuReg* quregs;

        public:
            GateStackFrame(GateAsmSymbol&, int);
            ~GateStackFrame();

            const GateAsmSymbol& getSymbol() const;
            int getReturnAddress() const;
            int* getIntRegs() const;
            QuReg* getQuRegs() const;
    };
}

#endif