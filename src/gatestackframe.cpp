#include "qudot/gatestackframe.h"

#include "qudot/bytecodes.h"

namespace qudot {
    GateStackFrame::GateStackFrame(GateAsmSymbol& _symbol, int _ra) : symbol(_symbol), return_address(_ra) {
        int_regs = new int[_symbol.getRegs() + _symbol.getArgs() + 1];
        quregs = new QuReg[_symbol.getQubitRegs() + 1];
    }

    GateStackFrame::~GateStackFrame() {
        delete[] int_regs;
        delete[] quregs;
    }

    const GateAsmSymbol& GateStackFrame::getSymbol() const {
        return symbol;
    }

    int* GateStackFrame::getIntRegs() const {
        return int_regs;
    }

    QuReg* GateStackFrame::getQuRegs() const {
        return quregs;
    }
}