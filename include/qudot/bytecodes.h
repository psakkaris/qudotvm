#ifndef __QUDOT_QUBYTECODES_H
#define __QUDOT_QUBYTECODES_H

#include <memory>
#include <string>

namespace qudot {
    namespace bytecodes {
        // supported bytecodes
        const unsigned char HALT = 0;
        const unsigned char PATHS = 1;
        const unsigned char X = 2;
        const unsigned char Y = 3;
        const unsigned char Z = 4;    // R(1)
        const unsigned char S = 5;    // R(2)
        const unsigned char T = 6;    // R(3)
        const unsigned char PHI = 7;
        const unsigned char H = 8;
        const unsigned char SWAP = 9;
        const unsigned char SWAP_AB = 10;
        const unsigned char MEASURE = 11;
        const unsigned char CNOT = 12;
        const unsigned char CROT = 13;
        const unsigned char SEMI_CNOT = 14;
        const unsigned char SEMI_CROT = 15;
        const unsigned char XON = 16;
        const unsigned char YON = 17;
        const unsigned char ZON = 18;
        const unsigned char SON = 19;
        const unsigned char TON = 20;
        const unsigned char PHION = 21;
        const unsigned char HON = 22;
        const unsigned char MON = 23;
        const unsigned char SWAPON = 24;
        const unsigned char QLOAD = 25;
        const unsigned char QLOAD_ARRAY = 26;
        const unsigned char IADD = 27;
        const unsigned char ISUB = 28;
        const unsigned char IMUL = 29;
        const unsigned char ILT = 30;
        const unsigned char IEQ = 31;
        const unsigned char INCR = 32;
        const unsigned char BR = 33;
        const unsigned char BRT = 34;
        const unsigned char BRF = 35;
        const unsigned char ILOAD = 36;
        const unsigned char RET = 37;
        const unsigned char MOVE = 38;
        const unsigned char QNULL = 39;
        const unsigned char CALL = 40;
        const unsigned char PRINTR = 41;
        const unsigned char QLOAD_SEQUENCE = 42;
        const unsigned char BREQ = 43;
        const unsigned char BRGEZ = 44;
        const unsigned char BRGTZ = 45;
        const unsigned char BRLEZ = 46;
        const unsigned char BRLTZ = 47;
        const unsigned char BRNEQ = 48;
        const unsigned char QLOADR = 49;
        const unsigned char IDIV = 50;
        const unsigned char DECR = 51;
        const unsigned char TOFF = 52;
        const unsigned char IQUADD = 53;
        const unsigned char QFT = 54;
        const unsigned char QFT_INV = 55;
        const unsigned char IQUADD_MOD = 56;
        const unsigned char IQUMUL_MOD = 57;
        const unsigned char CIQUADD_MOD = 58;
        const unsigned char CIQUMUL_MOD = 59;
        const unsigned char MODPOW = 60;
        const unsigned char PHIDAG = 61;
        const unsigned char PHIDAGON = 62;
        const unsigned char SDAG = 63;
        const unsigned char SDAGON = 64;
        const unsigned char TDAG = 65;
        const unsigned char TDAGON = 66;
        // end supported bytecodes
    }

    const int QUDOT_FILE_VERSION = 1;

    // const poll types
    enum ConstPoolType { GATE=1 };

    // libarary functions
    int getInt(const char memory[], int& index);
    int writeInt(char bytes[], int index, int value);

    struct GateAsmSymbolBytes {
        char* bytes;
        unsigned int size;
    };

    class GateAsmSymbol {
        private:
            std::string name;
            unsigned int args;
            unsigned int regs;
            unsigned int qubit_regs;
            unsigned int address;

        public:
            GateAsmSymbol(std::string);
            GateAsmSymbol(std::string, unsigned int, unsigned int, unsigned int, unsigned int);
            GateAsmSymbol(char bytes[], int ip=0);

            std::string getName() const;
            unsigned int getArgs() const;
            unsigned int getRegs() const;
            unsigned int getQubitRegs() const;
            unsigned int getAddress() const;
            GateAsmSymbolBytes getBytes() const;
    };
}

#endif