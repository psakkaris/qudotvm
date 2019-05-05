#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "tbb/tbb.h"
#include "qudot/qufrequency.h"

using namespace qudot;

TEST(QuFrequencyTest, testSerialFrequency) {
    QuFrequency freq(10000);

    std::string result1 = "hello";
    std::string result2 = "perry";
    std::string result3 = "loof";

    std::string no_result = "yoyo";

    for (int i=0; i < 1000; i++) {
        freq.addValue(result2);
    }
    for (int i=0; i < 6000; i++) {
        freq.addValue(result1);
    }
    for (int i=0; i < 3000; i++) {
        freq.addValue(result3);
    }

    ASSERT_EQ(freq.getCount(result1), 6000);
    ASSERT_EQ(freq.getCount(result2), 1000);
    ASSERT_EQ(freq.getCount(result3), 3000);
    ASSERT_EQ(freq.getCount(no_result), 0);
}
