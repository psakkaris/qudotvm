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

TEST(ThanosTest, quDnaTest) {
    ThanosUnit thanos;
    QuWorld* quworld1 = thanos.createWorld(6,1,5);
    QuWorld* quworld2 = thanos.createWorld(6, 1, 27);
    QuWorld* quworld3 = thanos.createWorld(6, 1, 33);
    QuWorld* quworld4 = thanos.createWorld(6, 1, 56);

    ASSERT_EQ(thanos.getQuDna(quworld1), "XXXYXY");
    ASSERT_EQ(thanos.getQuDna(quworld2), "XYYXYY");
    ASSERT_EQ(thanos.getQuDna(quworld3), "YXXXXY");
    ASSERT_EQ(thanos.getQuDna(quworld4), "YYYXXX");

    delete quworld1;
    delete quworld2;
    delete quworld3;
    delete quworld4;

    FeynmanUnit feynman;
    QuWorld* mixedworld = new QuWorld(6, 1, ONE_AMP64);
    feynman.x.applyGate(mixedworld, 6);
    feynman.h.applyGate(mixedworld, 1);
    feynman.h.applyGate(mixedworld, 4);

    ASSERT_EQ(thanos.getQuDna(mixedworld), "ZXXZXY");
    delete mixedworld;
}