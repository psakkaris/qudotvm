#include "gtest/gtest.h"
#include "qudot/bytecodes.h"
#include <iostream>

TEST(BytecodesTest, testReadWrite) {
    char memory[16];
    
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