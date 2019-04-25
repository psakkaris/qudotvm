#ifndef __QUDOT_KRATOSVM_H
#define __QUDOT_KRATOSVM_H

#include <cstddef>
#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "qudot/bytecodes.h"
#include "qudot/gatestackframe.h"
#include "qudot/qudot.h"
#include "qudot/qudotconfig.h"

namespace qudot {
    class KratosVM : public QuDot {
        private:
            size_t ip;
            int fp;
            char* code;
            std::vector<GateAsmSymbol> const_pool_gates;
            std::stack<std::unique_ptr<GateStackFrame>> calls;
            std::unique_ptr<GateAsmSymbol> main_gate;
            unsigned int num_qubits;
            unsigned int ensemble;
            int qudotc_fp;
            int bytecode_length;

        public:
            KratosVM(const std::string filename, const QuDotConfig& qudot_config);
            virtual ~KratosVM();

            void bohr();
            void getResults();
    };
}

#endif