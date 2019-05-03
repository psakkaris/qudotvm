#include "qudot/kratosvm.h"

#include <cstddef>
#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <string.h>

#include <qudot/bytecodes.h>
#include <qudot/qudotconfig.h>

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
        const_pool_gates.reserve(const_pool_size);
        for (int i=0; i < const_pool_size; i++) {
            int const_type = bytes[qudotc_fp++];
            if (const_type == 1) {
                int length = getInt(bytes, qudotc_fp);
                const_pool_gates[i] = GateAsmSymbol(bytes, qudotc_fp);
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
    }

    KratosVM::~KratosVM() { 
        delete[] code;
    }

    void KratosVM::bohr() { 
        if (!main_gate) {
            throw std::runtime_error("no main gate found");
        }
        GateStackFrame gsf = GateStackFrame(main_gate, ip);
        gsf.getIntRegs()[0] = num_qubits;
        calls.push(gsf);
        ip = main_gate->getAddress();
        feynmanProcessor();
    }

    void KratosVM::getResults() { }

    void KratosVM::feynmanProcessor() {
        char qu_code = code[ip];
        while (qu_code != bytecodes::HALT) {
            ip++;
            switch (qu_code) {
                case bytecodes::PATHS:
                    std::cout << "PATHS" << std::endl;
                    break;
                case bytecodes::X:
                    std::cout << "X" << std::endl;
                    break;
                case bytecodes::Y:
                    std::cout << "Y" << std::endl;
                    break;    
                case bytecodes::Z:
                    std::cout << "Z" << std::endl; 
                    break;
                case bytecodes::S:
                    std::cout << "S" << std::endl;
                    break;
                case bytecodes::T:
                    std::cout << "T" << std::endl;
                    break;
                case bytecodes::PHI:
                    std::cout << "PHI" << std::endl;
                    break;                   
                case bytecodes::H:
                    std::cout << "H" << std::endl;
                    break;
                case bytecodes::SWAP:
                    std::cout << "SWAP" << std::endl;    
                    break;
                case bytecodes::SWAP_AB:
                    std::cout << "SWAP_AB" << std::endl;
                    break;
                case bytecodes::MEASURE:
                    std::cout << "MEASURE" << std::endl;
                    break;
                case bytecodes::CNOT:
                    std::cout << "CNOT" << std::endl; 
                    break;
                case bytecodes::SEMI_CNOT:
                    std::cout << "SEMI_CNOT" << std::endl;
                    break;        
                case bytecodes::CROT:
                    std::cout << "CROT" << std::endl;
                    break;
                case bytecodes::SEMI_CROT:
                    std::cout << "CROT" << std::endl;
                    break;    
                case bytecodes::XON:
                    std::cout << "XON" << std::endl;  
                    break;
                case bytecodes::YON:
                    std::cout << "YON" << std::endl;                     
                    break;
                case bytecodes::ZON:
                    std::cout << "ZON" << std::endl;
                    break;
                case bytecodes::SON:
                    std::cout << "ZON" << std::endl;
                    break;
                case bytecodes::TON:
                    std::cout << "TON" << std::endl;
                    break;
                case bytecodes::PHION:
                    std::cout << "PHION" << std::endl;
                    break;                
                case bytecodes::HON:
                    std::cout << "HON" << std::endl;
                    break; 
                case bytecodes::MON:
                    std::cout << "MON" << std::endl;  
                    break;
                case bytecodes::SWAPON:
                    std::cout << "SWAPON" << std::endl;
                    break;
                case bytecodes::QLOAD:
                    std::cout << "QLOAD" << std::endl;
                    break;
                case bytecodes::QLOAD_ARRAY:
                    std::cout << "QLOAD_ARRAY" << std::endl;
                    break;
                case bytecodes::IADD:
                    std::cout << "IADD" << std::endl;
                    break;
                case bytecodes::ISUB:
                    std::cout << "ISUB" << std::endl; 
                    break;
                case bytecodes::IMUL:
                    std::cout << "IMUL" << std::endl; 
                    break;
                case bytecodes::ILT:
                    std::cout << "ILT" << std::endl;
                    break;
                case bytecodes::IEQ:
                    std::cout << "IEQ" << std::endl;
                    break;
                case bytecodes::INCR:
                    std::cout << "INCR" << std::endl;
                    break;
                case bytecodes::BR:
                    std::cout << "BR" << std::endl;
                    break;
                case bytecodes::BRT:
                    std::cout << "BRT" << std::endl;
                    break;
                case bytecodes::BRF:
                    std::cout << "BRF" << std::endl; 
                    break;                                                       
                case bytecodes::ILOAD:
                    std::cout << "ILOAD" << std::endl; 
                    break;
                case bytecodes::RET:
                    std::cout << "RET" << std::endl;
                    break;
                case bytecodes::QNULL:
                    std::cout << "QNULL" << std::endl;
                    break;        
                case bytecodes::MOVE:
                    std::cout << "MOVE" << std::endl;
                    break;
                case bytecodes::CALL:
                    std::cout << "CALL" << std::endl;  
                    break;
                case bytecodes::PRINTR:
                    std::cout << "PRINTR" << std::endl; 
                    break;
                case bytecodes::QLOAD_SEQUENCE:
                    std::cout << "QLOAD_SEQUENCE" << std::endl;
                    break;
                case bytecodes::BREQ:
                    std::cout << "BREQ" << std::endl;
                    break;
                case bytecodes::BRGEZ:
                    std::cout << "BRGEZ" << std::endl;
                    break;
                case bytecodes::BRGTZ:
                    std::cout << "BRGTZ" << std::endl;
                    break;
                case bytecodes::BRLEZ:
                    std::cout << "BRLEZ" << std::endl;
                    break;
                case bytecodes::BRLTZ:
                    std::cout << "BRLTZ" << std::endl;
                    break;
                case bytecodes::BRNEQ:
                    std::cout << "BRNEQ" << std::endl;
                    break;
                case bytecodes::QLOADR:
                    std::cout << "QLOADR" << std::endl;
                    break;
                case bytecodes::IDIV:
                    std::cout << "IDIV" << std::endl;
                    break;
                case bytecodes::DECR:
                    std::cout << "DECR" << std::endl;  
                    break;
                case bytecodes::TOFF:
                    std::cout << "TOFF" << std::endl;
                    break;
                case bytecodes::IQUADD:
                    std::cout << "IQUADD" << std::endl;
                    break;
                case bytecodes::QFT:
                    std::cout << "QFT" << std::endl;
                    break;
                case bytecodes::QFT_INV:
                    std::cout << "QFT_INV" << std::endl;
                    break;
                case bytecodes::IQUADD_MOD:
                    std::cout << "IQUADD_MOD" << std::endl; 
                    break;
                case bytecodes::IQUMUL_MOD:
                    std::cout << "IQUMUL_MOD" << std::endl;
                    break;
                case bytecodes::MODPOW:
                    std::cout << "MODPOW" << std::endl;
                    break;
                case bytecodes::PHIDAG:
                    std::cout << "PHIDAG" << std::endl;
                    break;
                case bytecodes::SDAG:
                    std::cout << "SDAG" << std::endl;
                    break;
                case bytecodes::SDAGON:
                    std::cout << "SDAGON" << std::endl; 
                    break;
                case bytecodes::TDAG:
                    std::cout << "TDAG" << std::endl;
                    break;
                case bytecodes::TDAGON:
                    std::cout << "TDAGON" << std::endl;     
                    break;                                                                              
                default:
                    break;    
            }
            qu_code = code[ip];
        }
    }
    //################### PRIVATE METHODS ####################
}
