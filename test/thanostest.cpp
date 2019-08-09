#include "gtest/gtest.h"
#include "qudot/components/thanosunit.h"
#include "qudottest.h"

#include <string>

using namespace qudot;

TEST(ThanosTest, createWorldTest) {
    ThanosUnit thanos;
    QuWorld* quworld = thanos.createWorld(6, 1, 27);

    std::string samples[10];
    sample(quworld, 10, samples);
    for (int i=0; i < 10; i++) {
        ASSERT_TRUE(samples[i] == "011011");
    }
    QuWorld* quworld2 = thanos.createWorld(6, 1, 33);

    sample(quworld2, 10, samples);
    for (int i=0; i < 10; i++) {
        ASSERT_TRUE(samples[i] == "100001");
    }

    QuWorld* quworld3 = thanos.createWorld(6, 1, 56);

    sample(quworld3, 10, samples);
    for (int i=0; i < 10; i++) {
        ASSERT_TRUE(samples[i] == "111000");
    }


    delete quworld;
    delete quworld2;    
    delete quworld3;
}