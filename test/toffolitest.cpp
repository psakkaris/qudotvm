#include "gtest/gtest.h"
#include "qudottest.h"

TEST(ToffoliTest, toff1) {
    std::string filename = "../resources/compiled/toffoli/toff1.qudotc";
    std::map<std::string, double> theory;
    theory["0000"] = .25;
    theory["0010"] = .125;
    theory["0011"] = .125;
    theory["1000"] = .120;
    theory["1001"] = .0048;
    theory["1010"] = .038;
    theory["1011"] = .086;
    theory["1100"] = .038;
    theory["1101"] = .086;
    theory["1110"] = .120;
    theory["1111"] = .0048;

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));        
}

TEST(ToffoliTest, toff2) {
    std::string filename = "../resources/compiled/toffoli/toff2.qudotc";
    std::map<std::string, double> theory;
    theory["0000"] = .25;
    theory["0010"] = .125;
    theory["0011"] = .125;
    theory["1000"] = .120;
    theory["1001"] = .0048;
    theory["1010"] = .086;
    theory["1011"] = .038;
    theory["1100"] = .086;
    theory["1101"] = .038;
    theory["1110"] = .120;
    theory["1111"] = .0048;

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));        
}

TEST(ToffoliTest, toff3) {
    std::string filename = "../resources/compiled/toffoli/toff3.qudotc";
    std::map<std::string, double> theory;
    theory["0000"] = .25;
    theory["0010"] = .125;
    theory["0011"] = .125;
    theory["1000"] = .120;
    theory["1001"] = .0048;
    theory["1010"] = .038;
    theory["1011"] = .086;
    theory["1100"] = .0048;
    theory["1101"] = .120;
    theory["1110"] = .086;
    theory["1111"] = .038;

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));        
}