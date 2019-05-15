#include "gtest/gtest.h"
#include "qudottest.h"

TEST(QFTIntrinsicsTest, qft34Intrinsic) {
    std::string filename = "../resources/compiled/qft/qft34Intrinsic.qudotc";
    std::map<std::string, double> theory;
    theory["0000"] = .25;
    theory["0001"] = .205;
    theory["0010"] = .1066;
    theory["0011"] = .025;
    theory["0101"] = .0113;
    theory["0110"] = .0183;
    theory["0111"] = .00812;
    theory["1001"] = .00812;
    theory["1010"] = .0183;
    theory["1011"] = .0113;
    theory["1101"] = .025;
    theory["1110"] = .1066;
    theory["1111"] = .205;

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));    
}