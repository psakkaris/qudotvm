#include "qudot/gatestackframe.h"

#include <iostream>
#include <memory>

#include "qudot/bytecodes.h"

namespace qudot {
    GateStackFrame::GateStackFrame(std::shared_ptr<GateAsmSymbol> asm_symbol, int _ra) : 
            symbol(asm_symbol), return_address(_ra), int_regs(symbol->getRegs() + symbol->getArgs() + 1, 0),
            quregs(symbol->getQubitRegs() + 1) {

    }

    std::shared_ptr<GateAsmSymbol> GateStackFrame::getSymbol() const {
        return symbol;
    }

    const std::vector<int>& GateStackFrame::getIntRegs() const {
        return int_regs;
    }

    const std::vector<QuReg>& GateStackFrame::getQuRegs() const {
        return quregs;
    }

    int GateStackFrame::getReturnAddress() const {
        return return_address;
    }

    void GateStackFrame::setIntReg(const int i, const int val) {
        int_regs[i] = val;
    }

    void GateStackFrame::addQuReg(const int i, const int val) {
        quregs[i].addQubit(val);
    }
}