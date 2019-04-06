#include "gtest/gtest.h"

int subtract(int val1, int val2) {
    return val1 - val2;
}

TEST(SubtractTest, SubtractTwoNumbers)
{
    EXPECT_EQ(5, subtract(6, 1));
}