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

TEST(SwapTest, qft24Swap1) {
    std::string filename = "../resources/compiled/swap/qft24_swap1.qudotc";
    std::map<std::string, double> theory;
    theory["0000"] = .25;
    theory["0100"] = .125;
    theory["0101"] = .125;
    theory["1000"] = .120;
    theory["1001"] = .0048;
    theory["1010"] = .086;
    theory["1011"] = .0386;
    theory["1100"] = .0386;
    theory["1101"] = .086;
    theory["1110"] = .0048;
    theory["1111"] = .120;     

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));       
}

TEST(SwapTest, iqft34Swap) {
    std::string filename = "../resources/compiled/swap/iqft34_full_swap.qudotc";
    std::map<std::string, double> theory;
    theory["0000"] = .25;
    theory["0001"] = .205;
    theory["0010"] = .1066;
    theory["0011"] = .0253;
    theory["0101"] = .0113;
    theory["0110"] = .0183;
    theory["0111"] = .0081;
    theory["1001"] = .0081;
    theory["1010"] = .0183;
    theory["1011"] =  .0113;
    theory["1101"] = .0253;
    theory["1110"] = .1066;
    theory["1111"] = .205;    

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));       
}