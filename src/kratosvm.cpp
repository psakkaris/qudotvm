#include "qudot/kratosvm.h"

#include <cstddef>
#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <string.h>

#include <boost/multiprecision/cpp_int.hpp>

#include "qudot/bytecodes.h"
#include "qudot/common.h"
#include "qudot/qudotconfig.h"
#include "qudot/quworld.h"
#include "qudot/components/heisenbergunit.h"
#include "qudot/intrinsics/qft.h"
#include "qudot/intrinsics/qftinv.h"

namespace qudot {
    KratosVM::KratosVM(const std::string filename, const QuDotConfig& qc) : qudotc_fp(0) {
        num_qubits = qc.getNumQubits();
        ensemble = qc.getEnsembleSize();

        std::ifstream infile;
        infile.open(filename, std::ios::in|std::ios::binary|std::ios::ate);
        size_t file_size = infile.tellg();

        infile.seekg(0, std::ios::beg);
        char* bytes = new char[file_size+1];
        infile.read(bytes, file_size);

        int file_version = getInt(bytes, qudotc_fp);
        if (file_version != QUDOT_FILE_VERSION) {
            throw std::runtime_error("Invalid QuDot Bytecode File");
        }
        // command line to override num qubits and ensemble size?
        int _num_qubits = getInt(bytes, qudotc_fp);
        if (num_qubits == 0) {
            num_qubits = _num_qubits;
        }
        int _ensemble = getInt(bytes, qudotc_fp);
        if (ensemble == 0) {
            ensemble = _ensemble;
        }
        // get main gate
        if(bytes[qudotc_fp++] != GATE) {
            throw std::runtime_error("Invalid main gate");
        }
        int gate_length = getInt(bytes, qudotc_fp);
        main_gate = std::make_shared<GateAsmSymbol>(bytes, qudotc_fp);
        qudotc_fp += gate_length;

        // read in constants
        int const_pool_size = getInt(bytes, qudotc_fp);
        for (int i=0; i < const_pool_size; i++) {
            int const_type = bytes[qudotc_fp++];
            if (const_type == 1) {
                int length = getInt(bytes, qudotc_fp);
                auto cg = std::make_shared<GateAsmSymbol>(bytes, qudotc_fp);
                const_pool_gates.push_back(cg);
                qudotc_fp += length;
            } else {
                throw std::runtime_error("Invalid constant type");
            } 
        }

        bytecode_length = file_size + 1 - qudotc_fp + 1;
        code = new char[bytecode_length];
        code[bytecode_length-1] = 0;
        memcpy(code, &bytes[qudotc_fp], bytecode_length);
        std::cout << "bytecode length: " << bytecode_length << std::endl;

        //qu_world = new QuWorld(num_qubits, 1, ONE_AMP64);
        qumvn = std::make_shared<QuMvN>(num_qubits, 1);
        delete[] bytes;
    }

    KratosVM::~KratosVM() { 
        if (code) delete[] code;
        //if (qu_world) delete qu_world;

        code = nullptr;
        //qu_world = nullptr;
    }

    KratosVM::KratosVM(const KratosVM & other) {
        std::cout << "I'm making a copy!\n";
    }

    KratosVM& KratosVM::operator=(KratosVM& other) {
        std::cout << "I'm making an assignment!\n";
    }

    void KratosVM::bohr() { 
        if (!main_gate) {
            throw std::runtime_error("no main gate found");
        }
        auto gsf = std::make_shared<GateStackFrame>(main_gate, ip);
        gsf->setIntReg(0, num_qubits);
        calls.push(gsf);
        ip = main_gate->getAddress();
        feynmanProcessor();
    }

    void KratosVM::getResults(QuFrequency& freq) { 
        std::cout << "num worlds: " << qumvn->size() << "\n";
        HeisenbergUnit::getResults(qumvn.get(), ensemble, freq);
    }

    unsigned int KratosVM::getEnsemble() const {
        return ensemble;
    }

    void KratosVM::feynmanProcessor() {
        char qu_code = code[ip];
        int qureg_index;
        int value;
        int k;
        int r1, r2, r3, r4;
        Qft qft;
        QftInv qft_inv;
        QuReg qureg1, qureg2, qureg3;
        boost::multiprecision::cpp_int big_result, base, mod, pow;

        while (qu_code != bytecodes::HALT) {
            ip++;
            auto gsf = calls.top();
            const std::vector<int> int_regs = gsf->getIntRegs();
            const std::vector<QuReg> quregs = gsf->getQuRegs();

            switch (qu_code) {
                case bytecodes::PATHS:
                    printWorlds(std::cout);
                    break;
                case bytecodes::PRINTR:
                    r1 = getInt(code, ip);
                    std::cout << int_regs[r1] << std::endl; 
                    break;
                case bytecodes::X:
                    applyGateToQuMvN(feynmanUnit.x);
                    break;
                case bytecodes::XON:
                    applyGateToQuMvN(feynmanUnit.x, quregs);
                    break;                    
                case bytecodes::Y:
                    applyGateToQuMvN(feynmanUnit.y);
                    break; 
                case bytecodes::YON:
                    applyGateToQuMvN(feynmanUnit.y, quregs);                                        
                    break;                       
                case bytecodes::Z:
                    applyGateToQuMvN(feynmanUnit.z);
                    break;
                case bytecodes::ZON:
                    applyGateToQuMvN(feynmanUnit.z, quregs);                   
                    break;
                case bytecodes::S:
                    applyGateToQuMvN(feynmanUnit.s);
                    break;
                case bytecodes::SON:
                    applyGateToQuMvN(feynmanUnit.s, quregs);   
                    break;                    
                case bytecodes::T:
                    applyGateToQuMvN(feynmanUnit.t);
                    break;
                case bytecodes::TON:
                    applyGateToQuMvN(feynmanUnit.t, quregs);
                    break;
                case bytecodes::PHI:
                    k = getInt(code, ip);
                    applyGateToQuMvN(*feynmanUnit.getRGate(k));
                    break;      
                case bytecodes::PHION:
                    k = getInt(code, ip);
                    applyGateToQuMvN(*feynmanUnit.getRGate(k), quregs);
                    break;     
                case bytecodes::PHIDAG:
                    k = getInt(code, ip);
                    applyGateToQuMvN(*feynmanUnit.getRDaggerGate(k));
                    break;
                case bytecodes::PHIDAGON:
                    k = getInt(code, ip);
                    applyGateToQuMvN(*feynmanUnit.getRDaggerGate(k), quregs);                
                    break;    
                case bytecodes::SDAG:
                    applyGateToQuMvN(feynmanUnit.sdag);
                    break;
                case bytecodes::SDAGON:
                    applyGateToQuMvN(feynmanUnit.sdag, quregs);
                    break;
                case bytecodes::TDAG:
                    applyGateToQuMvN(feynmanUnit.tdag);
                    break;
                case bytecodes::TDAGON:
                    applyGateToQuMvN(feynmanUnit.tdag, quregs);     
                    break;           
                case bytecodes::H:
                    applyGateToQuMvN(feynmanUnit.h);
                    break;
                case bytecodes::HON:
                    applyGateToQuMvN(feynmanUnit.h, quregs);                   
                    break;                     
                case bytecodes::SWAP:
                    qumvn->swap();   
                    break;
                case bytecodes::SWAP_AB:
                    qureg1 = quregs[getInt(code, ip)];
                    qureg2 = quregs[getInt(code, ip)];

                    qumvn->swap(qureg1.getQubits()[0], qureg2.getQubits()[0]);
                    break;
                case bytecodes::SWAPON:
                    std::cout << "SWAPON" << std::endl;
                    break;
                case bytecodes::MEASURE:
                    std::cout << "MEASURE" << std::endl;
                    break;
                case bytecodes::MON:
                    qureg1 = quregs[getInt(code, ip)];
                    std::cout << "MON" << std::endl;  
                    break;
                case bytecodes::CNOT:
                    qureg1 = quregs[getInt(code, ip)];
                    qureg2 = quregs[getInt(code, ip)];

                    feynmanUnit.x.applyControlGate(qumvn.get(), qureg2.getQubits()[0], qureg1.getQubits());
                    break; 
                case bytecodes::SEMI_CNOT:
                    qureg1 = quregs[getInt(code, ip)];
                    qureg2 = quregs[getInt(code, ip)];

                    feynmanUnit.x.applyControlGate(qumvn.get(), qureg2.getQubits()[0], qureg1.getQubits());
                    break;      
                case bytecodes::CROT:
                    r1 = int_regs[getInt(code, ip)];
                    qureg1 = quregs[getInt(code, ip)];
                    qureg2 = quregs[getInt(code, ip)];

                    feynmanUnit.getRGate(r1)->applyControlGate(qumvn.get(), qureg2.getQubits()[0], qureg1.getQubits());
                    break; 
                case bytecodes::SEMI_CROT:
                    r1 = int_regs[getInt(code, ip)];
                    qureg1 = quregs[getInt(code, ip)];
                    qureg2 = quregs[getInt(code, ip)];

                    feynmanUnit.getRGate(r1)->applyControlGate(qumvn.get(), qureg2.getQubits()[0], qureg1.getQubits());
                    break;    
                case bytecodes::TOFF:
                    qureg1 = quregs[getInt(code, ip)];
                    qureg2 = quregs[getInt(code, ip)];

                    feynmanUnit.x.QuGate::applyControlGate(qumvn.get(), qureg2.getQubits()[0], qureg1.getQubits());
                    break;              
                case bytecodes::QLOAD:
                    qureg_index = getInt(code, ip);
                    value = getInt(code, ip);
                    gsf->addQuReg(qureg_index, value);
                    break;
                case bytecodes::QLOAD_ARRAY:
                    qureg_index = getInt(code, ip);
                    r1 = getInt(code, ip);
                    for (int i=0; i < r1; i++) {
                        value = getInt(code, ip);
                        gsf->addQuReg(qureg_index, value);
                    }
                    break;
                case bytecodes::QLOAD_SEQUENCE:
                    qureg_index = getInt(code, ip);
                    r1 = getInt(code, ip);
                    r2 = getInt(code, ip);

                    for (int i=r1; i <= r2; i++) {
                        gsf->addQuReg(qureg_index, i);
                    }
                    break;
                case bytecodes::QLOADR:
                    qureg_index = getInt(code, ip);
                    r1 = getInt(code, ip);
                    gsf->setQuReg(qureg_index, int_regs[r1]);
                    break;
                case bytecodes::ILOAD:
                    r1 = getInt(code, ip);
                    value = getInt(code, ip);
                    gsf->setIntReg(r1, value);
                    break;
                case bytecodes::IADD:
                    r3 = getInt(code, ip);
                    r1 = getInt(code, ip);
                    r2 = getInt(code, ip);

                    gsf->setIntReg(r3, int_regs[r1] + int_regs[r2]);
                    break;
                case bytecodes::ISUB:
                    r3 = getInt(code, ip);
                    r1 = getInt(code, ip);
                    r2 = getInt(code, ip);
                    
                    gsf->setIntReg(r3, int_regs[r1] - int_regs[r2]);
                    break;
                case bytecodes::IMUL:
                    r3 = getInt(code, ip);
                    r1 = getInt(code, ip);
                    r2 = getInt(code, ip);

                    gsf->setIntReg(r3, int_regs[r1] * int_regs[r2]);
                    break;
                case bytecodes::IDIV:
                    r3 = getInt(code, ip);
                    r1 = getInt(code, ip);
                    r2 = getInt(code, ip);

                    gsf->setIntReg(r3, int_regs[r1] / int_regs[r2]);
                    break;
                case bytecodes::INCR:
                    value = getInt(code, ip);
                    gsf->setIntReg(value, int_regs[value] + 1);
                    break;   
                case bytecodes::DECR:
                    value = getInt(code, ip);
                    gsf->setIntReg(value, int_regs[value] - 1);  
                    break;                 
                case bytecodes::ILT:
                    r3 = getInt(code, ip);
                    r1 = getInt(code, ip);
                    r2 = getInt(code, ip);

                    gsf->setIntReg(r3, int_regs[r1] < int_regs[r2]);
                    break;
                case bytecodes::IEQ:
                    r3 = getInt(code, ip);
                    r1 = getInt(code, ip);
                    r2 = getInt(code, ip);

                    gsf->setIntReg(r3, int_regs[r1] == int_regs[r2]);                    
                    break;

                case bytecodes::BR:
                    ip = getInt(code, ip);
                    break;
                case bytecodes::BRT:
                    r1 = getInt(code, ip);
                    value = getInt(code, ip);
                    if (int_regs[r1]) {
                        ip = value;
                    }
                    break;
                case bytecodes::BRF:
                    r1 = getInt(code, ip);
                    value = getInt(code, ip);
                    if (!int_regs[r1]) {
                        ip = value;
                    }
                    break; 
                case bytecodes::BREQ:
                    r1 = getInt(code, ip);
                    r2 = getInt(code, ip);
                    value = getInt(code, ip);
                    if (int_regs[r1] == int_regs[r2]) {
                        ip = value;
                    }
                    break;
                case bytecodes::BRGEZ:
                    r1 = getInt(code, ip);
                    value = getInt(code, ip);
                    if (int_regs[r1] >= 0) {
                        ip = value;
                    }
                    break;
                case bytecodes::BRGTZ:
                    r1 = getInt(code, ip);
                    value = getInt(code, ip);
                    if (int_regs[r1] > 0) {
                        ip = value;
                    }
                    break;
                case bytecodes::BRLEZ:
                    r1 = getInt(code, ip);
                    value = getInt(code, ip);
                    if (int_regs[r1] <= 0) {
                        ip = value;
                    }
                    break;
                case bytecodes::BRLTZ:
                    r1 = getInt(code, ip);
                    value = getInt(code, ip);
                    if (int_regs[r1] < 0) {
                        ip = value;
                    }
                    break;
                case bytecodes::BRNEQ:
                    r1 = getInt(code, ip);
                    r2 = getInt(code, ip);
                    value = getInt(code, ip);
                    if (int_regs[r1] != int_regs[r2]) {
                        ip = value;
                    }
                    break;                                                      
                case bytecodes::RET:
                    // pop stack frame
                    ip = gsf->getReturnAddress();
                    // return value
                    value = int_regs[0];
                    calls.pop();
                    calls.top()->setIntReg(0, value);

                    break;
                case bytecodes::QNULL:
                    r1 = getInt(code, ip);
                    gsf->setIntReg(r1, -1);
                    break;        
                case bytecodes::MOVE:
                    r1 = getInt(code, ip);
                    r2 = getInt(code, ip);

                    gsf->setIntReg(r1, int_regs[r2]);
                    break;
                case bytecodes::CALL:
                    r1 = getInt(code, ip);
                    r2 = getInt(code, ip);
                    call(r1, r2); 
                    break;
                case bytecodes::IQUADD:
                    std::cout << "IQUADD" << std::endl;
                    break;
                case bytecodes::QFT:
                    qureg1 = quregs[getInt(code, ip)];
                    qureg2 = quregs[getInt(code, ip)];

                    qft(qumvn.get(), feynmanUnit, qureg1.getQubits()[0], qureg2.getQubits()[0]);
                    break;
                case bytecodes::QFT_INV:
                    qureg1 = quregs[getInt(code, ip)];
                    qureg2 = quregs[getInt(code, ip)];

                    qft_inv(qumvn.get(), feynmanUnit, qureg1.getQubits()[0], qureg2.getQubits()[0]);
                    break;
                case bytecodes::IQUADD_MOD:
                    std::cout << "IQUADD_MOD" << std::endl; 
                    break;
                case bytecodes::IQUMUL_MOD:
                    std::cout << "IQUMUL_MOD" << std::endl;
                    break;
                case bytecodes::MODPOW:
                    r1 = getInt(code, ip);
                    r2 = getInt(code, ip);
                    r3 = getInt(code, ip);
                    r4 = getInt(code, ip);

                    base.assign(int_regs[r2]);
                    mod.assign(int_regs[r4]);
                    pow = boost::multiprecision::pow(static_cast<boost::multiprecision::cpp_int>(2), int_regs[r3]); 
                    big_result = boost::multiprecision::powm(base, pow, mod);

                    gsf->setIntReg(r1, big_result.convert_to<int>());
                    break;                                                                             
                default:
                    break;    
            }
            qu_code = code[ip];
        }
    }

    void KratosVM::printWorlds(const std::string& filename) {
        std::ofstream myfile;
        myfile.open(filename);  
        printWorlds(myfile);      
    }

    void KratosVM::printWorlds(std::ostream& out) {
        out << *(qumvn.get()) << "\n";
    }
    //################### PRIVATE METHODS ####################
    void KratosVM::printQuReg(const QuReg& qr) {
        auto qubits = qr.getQubits();
        for (auto it=qubits.begin(); it != qubits.end(); ++it) {
            std::cout << *it << ", ";
        }
    }

    void KratosVM::applyGateToQuMvN(QuGate& qugate) {
        for (unsigned int i=1; i <= num_qubits; i++) {
            qugate.applyGate(qumvn.get(), i);
            //qugate.applyGate(qu_world, i);
        }    
    }   

    void KratosVM::applyGateToQuMvN(QuGate& qugate, const std::vector<QuReg>& quregs) {
        int index = getInt(code, ip);
        auto qubits = quregs[index].getQubits();
        for (auto it=qubits.begin(); it != qubits.end(); ++it) {
            //printf("applying %s to qubit: %d\n", qugate.getId().c_str(), *it);
            qugate.applyGate(qumvn.get(), *it);
            //qugate.applyGate(qu_world, *it);
        }
    }

    void KratosVM::call(const int gate_index, const int first_reg_index) {
        auto gate_symbol = const_pool_gates[gate_index];
        auto f = std::make_shared<GateStackFrame>(gate_symbol, ip);
        f->setIntReg(0, num_qubits);
        auto calling_frame = calls.top();

        // move arguments from calling stack to new stack frame
        for (unsigned int arg=0; arg < gate_symbol->getArgs(); arg++) {
            // move args, leaving room for r0
            f->setIntReg(arg+1, calling_frame->getIntRegs()[first_reg_index+arg]);
            //std::cout << "r" << arg+1 << "=" << calling_frame->getIntRegs()[first_reg_index+arg] << "\n";
        }
        // push new stack to frame
        calls.push(f);
        // branch to gate frame
        ip = gate_symbol->getAddress();
    } 
}
