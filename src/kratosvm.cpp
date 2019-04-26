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
                case bytecodes::H:
                    std::cout << "H" << std::endl;
                    break;
                case bytecodes::HON:
                    std::cout << "HON" << std::endl;    
                case bytecodes::ILOAD:
                    std::cout << "ILOAD" << std::endl; 
                case bytecodes::MOVE:
                    std::cout << "MOVE" << std::endl;
                case bytecodes::CALL:
                    std::cout << "CALL" << std::endl;
                case bytecodes::PATHS:
                    std::cout << "PATHS" << std::endl;    
                default:
                    break;    
            }
            qu_code = code[ip];
        }
    }
    //################### PRIVATE METHODS ####################
}
