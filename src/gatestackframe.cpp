#include "qudot/gatestackframe.h"

#include <memory>

#include "qudot/bytecodes.h"

namespace qudot {
    GateStackFrame::GateStackFrame(std::shared_ptr<GateAsmSymbol> asm_symbol, int _ra) : return_address(_ra) {
        symbol = asm_symbol;
        int_regs = new int[symbol->getRegs() + symbol->getArgs() + 1];
        quregs = new QuReg[symbol->getQubitRegs() + 1];
    }

    GateStackFrame::~GateStackFrame() {
        delete[] int_regs;
        delete[] quregs;
    }

    std::shared_ptr<GateAsmSymbol> GateStackFrame::getSymbol() const {
        return symbol;
    }

    int* GateStackFrame::getIntRegs() const {
        return int_regs;
    }

    QuReg* GateStackFrame::getQuRegs() const {
        return quregs;
    }
}