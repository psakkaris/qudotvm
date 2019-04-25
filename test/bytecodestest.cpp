#include "gtest/gtest.h"
#include "qudot/bytecodes.h"
#include <iostream>

TEST(BytecodesTest, testReadWrite) {
    unsigned char memory[16];
    
    int index1 = qudot::writeInt(memory, 0, 2897);
    int index2 = qudot::writeInt(memory, 4, -28);
    int index3 = qudot::writeInt(memory, 8, 129347);
    int index4 = qudot::writeInt(memory, 12, -527);
    ASSERT_EQ(qudot::getInt(memory, 0), 2897);
    ASSERT_EQ(qudot::getInt(memory, index1), -28);
    ASSERT_EQ(qudot::getInt(memory, index2), 129347);
    ASSERT_EQ(qudot::getInt(memory, index3), -527);

    ASSERT_EQ(index4, 16);
}

/*
       GateAsmSymbol symbol = new GateAsmSymbol("Main123Feynman4", 5, 99, 23, 1290);
        byte[] symbolBytes = symbol.getBytes();
        assertEquals(symbolBytes.length, 35);
        assertEquals(BytecodeUtils.getInt(symbolBytes, 0), 15);

        GateAsmSymbol deSymbol = GateAsmSymbol.fromBytes(symbolBytes);
        assertEquals(symbol, deSymbol);
        assertEquals(symbol.getArgs(), deSymbol.getArgs());
        assertEquals(symbol.getRegs(), deSymbol.getRegs());
        assertEquals(symbol.getQubitRegs(), deSymbol.getQubitRegs());
        assertEquals(symbol.getAddress(), deSymbol.getAddress());
        */
TEST(BytecodesTest, testGateAsmSymbol) {
    qudot::GateAsmSymbol symbol("Main123Feynman4", 55, 99, 23, 1290);
    qudot::GateAsmSymbolBytes symbolBytes = symbol.getBytes();
    ASSERT_EQ(qudot::getInt(symbolBytes.bytes, 0), 15);
    ASSERT_EQ(symbolBytes.size, 35);
    
    auto daSymbol = qudot::GateAsmSymbol(symbolBytes.bytes);
    ASSERT_EQ(symbol.getArgs(), daSymbol.getArgs());
    ASSERT_EQ(symbol.getRegs(), daSymbol.getRegs());
    ASSERT_EQ(symbol.getQubitRegs(), daSymbol.getQubitRegs());
    ASSERT_EQ(symbol.getAddress(), daSymbol.getAddress());

    ASSERT_EQ(symbol.getName(), daSymbol.getName());
    delete[] symbolBytes.bytes;
}