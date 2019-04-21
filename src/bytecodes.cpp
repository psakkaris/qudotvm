#include "bytecodes.h"

int qudot::getInt(char memory[], int index) {
    int b1 = memory[index++]&0xFF; // mask off sign-extended bits
    int b2 = memory[index++]&0xFF;
    int b3 = memory[index++]&0xFF;
    int b4 = memory[index++]&0xFF;
    int word = b1<<(8*3) | b2<<(8*2) | b3<<(8*1) | b4;
        
    return word;    
}

int qudot::writeInt(char bytes[], int index, int value) {
        bytes[index+0] = ((value>>(8*3))&0xFF); // get highest byte
        bytes[index+1] = ((value>>(8*2))&0xFF);
        bytes[index+2] = ((value>>(8*1))&0xFF);
        bytes[index+3] = (value&0xFF);

        return index + 4;
}