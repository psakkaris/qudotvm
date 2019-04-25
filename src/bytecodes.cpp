#include "qudot/bytecodes.h"

#include <cstddef>
#include <memory>
#include <string>

namespace qudot {
    int getInt(const unsigned char memory[], int index) {
        int b1 = memory[index++]&0xFF; // mask off sign-extended bits
        int b2 = memory[index++]&0xFF;
        int b3 = memory[index++]&0xFF;
        int b4 = memory[index++]&0xFF;
        int word = b1<<(8*3) | b2<<(8*2) | b3<<(8*1) | b4;
                
        return word;    
    }

    int writeInt(unsigned char bytes[], int index, int value) {
        bytes[index+0] = ((value>>(8*3))&0xFF); // get highest byte
        bytes[index+1] = ((value>>(8*2))&0xFF);
        bytes[index+2] = ((value>>(8*1))&0xFF);
        bytes[index+3] = (value&0xFF);

        return index + 4;
    }

    GateAsmSymbol::GateAsmSymbol(std::string _name) : name(_name), args(0), regs(0), qubit_regs(0), address(0) {}

    GateAsmSymbol::GateAsmSymbol(std::string _name, unsigned int _args, unsigned int _regs, unsigned int _qubit_regs, unsigned int _address) :
        name(_name), args(_args), regs(_regs), qubit_regs(_qubit_regs), address(_address) {}

    GateAsmSymbol::GateAsmSymbol(unsigned char bytes[]) {
        int ip=0;
        unsigned int name_length = getInt(bytes, ip);
        ip += 4;
        char name_arr[name_length+1];
        for (std::size_t i=0; i < name_length; i++) {
                name_arr[i] = bytes[ip++];
        }         
        name = std::string(name_arr);   
        args = getInt(bytes, ip);
        ip += 4;
        regs = getInt(bytes, ip);
        ip += 4;
        qubit_regs = getInt(bytes, ip);
        ip += 4;
        address = getInt(bytes, ip);
        ip += 4;        
    }    

    std::string GateAsmSymbol::getName() const {
        return name;
    }

    unsigned int GateAsmSymbol::getArgs() const {
        return args;
    }

    unsigned int GateAsmSymbol::getRegs() const {
        return regs;        
    }

    unsigned int GateAsmSymbol::getQubitRegs() const {
        return qubit_regs;    
    }

    unsigned int GateAsmSymbol::getAddress() const {
        return address;    
    }

    GateAsmSymbolBytes GateAsmSymbol::getBytes() const {
        unsigned int ip=0;

        unsigned char* bytes = new unsigned char[name.size() + 20];
        const char* name_bytes = name.c_str();

        ip = writeInt(bytes, ip, name.size()); 
        for (std::size_t i=0; i < name.size(); i++) {
            bytes[ip++] = name_bytes[i];
        }
        ip = writeInt(bytes, ip, args);
        ip = writeInt(bytes, ip, regs);
        ip = writeInt(bytes, ip, qubit_regs);
        ip = writeInt(bytes, ip, address);

        GateAsmSymbolBytes symbolBytes = {bytes, ip};

        return symbolBytes;
    }
} // namespace qudot