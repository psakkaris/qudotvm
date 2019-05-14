#include "gtest/gtest.h"

#include "qudottest.h"

#include <map>
#include <string>

TEST(SwapTest, fullSwap) {
    std::string filename = "../resources/compiled/swap/swap2.qudotc";
    std::map<std::string, double> theory;
    theory["1010110010"] = .25;
    theory["1010110011"] = .25;
    theory["1010110110"] = .25;
    theory["1010110111"] = .25;

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));    
}

TEST(SwapTest, abSwap) {
    std::string filename = "../resources/compiled/swap/swap1.qudotc";
    std::map<std::string, double> theory;
    theory["0100"] = .5;
    theory["1100"] = .5;      

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));     
}