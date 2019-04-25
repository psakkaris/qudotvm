#include "gtest/gtest.h"
#include "qudot/bytecodes.h"
#include <iostream>

TEST(BytecodesTest, testReadWrite) {
    char memory[16];
    int index = 0;
    int index1 = qudot::writeInt(memory, 0, 2897);
    int index2 = qudot::writeInt(memory, 4, -28);
    int index3 = qudot::writeInt(memory, 8, 129347);
    int index4 = qudot::writeInt(memory, 12, -527);
    ASSERT_EQ(qudot::getInt(memory, index), 2897);
    ASSERT_EQ(qudot::getInt(memory, index1), -28);
    ASSERT_EQ(qudot::getInt(memory, index2), 129347);
    ASSERT_EQ(qudot::getInt(memory, index3), -527);

    ASSERT_EQ(index4, 16);
}

TEST(BytecodesTest, testGateAsmSymbol) {
    int start = 0;
    qudot::GateAsmSymbol symbol("Main123Feynman4", 55, 99, 23, 1290);
    qudot::GateAsmSymbolBytes symbolBytes = symbol.getBytes();
    ASSERT_EQ(qudot::getInt(symbolBytes.bytes, start), 15);
    ASSERT_EQ(symbolBytes.size, 35);
    
    auto daSymbol = qudot::GateAsmSymbol(symbolBytes.bytes);
    ASSERT_EQ(symbol.getArgs(), daSymbol.getArgs());
    ASSERT_EQ(symbol.getRegs(), daSymbol.getRegs());
    ASSERT_EQ(symbol.getQubitRegs(), daSymbol.getQubitRegs());
    ASSERT_EQ(symbol.getAddress(), daSymbol.getAddress());

    ASSERT_EQ(symbol.getName(), daSymbol.getName());
    delete[] symbolBytes.bytes;
}