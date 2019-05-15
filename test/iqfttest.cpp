#include "gtest/gtest.h"
#include "qudottest.h"

TEST(IQFTTest, iqft12) {
    std::string filename = "../resources/compiled/iqft/iqft12.qudotc";
    std::map<std::string, double> theory;
    theory["0000"] = .25;
    theory["0100"] = .25;
    theory["1000"] = .25;
    theory["1100"] = .25;

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));
}

TEST(IQFTTest, iqft13) {
    std::string filename = "../resources/compiled/iqft/iqft13.qudotc";
    std::map<std::string, double> theory;
    theory["0000"] = .25;
    theory["0010"] = .125;
    theory["0110"] = .125;
    theory["1000"] = .25;
    theory["1010"] = .125;
    theory["1110"] = .125;

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));
}

TEST(IQFTTest, iqft14) {
    std::string filename = "../resources/compiled/iqft/iqft14.qudotc";
    std::map<std::string, double> theory;
    theory["0000"] = .25;
    theory["0010"] = .213;
    theory["0100"] = .125;
    theory["0110"] = .0366;
    theory["1010"] = .0366;
    theory["1100"] = .125;
    theory["1110"] = .213;

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));
}

TEST(IQFTTest, iqft23) {
    std::string filename = "../resources/compiled/iqft/iqft23.qudotc";
    std::map<std::string, double> theory;
    theory["0000"] = .25;
    theory["0001"] = .1066;
    theory["0011"] = .0183;
    theory["0101"] = .0183;
    theory["0111"] = .1066;
    theory["1000"] = .25;
    theory["1001"] = .1066;
    theory["1011"] = .0183;
    theory["1101"] = .0183;
    theory["1111"] = .1066;

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));
}

TEST(IQFTTest, iqft24) {
    std::string filename = "../resources/compiled/iqft/iqft24.qudotc";
    std::map<std::string, double> theory;
    theory["0000"] = .25;
    theory["0001"] = .120;
    theory["0011"] = .086;
    theory["0100"] = .125;
    theory["0101"] = .0385;
    theory["0111"] = .00475;
    theory["1001"] = .00475;
    theory["1011"] = .0385;
    theory["1100"] = .125;
    theory["1101"] = .086;
    theory["1111"] = .120;

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));
}

TEST(IQFTTest, iqft34) {
    std::string filename = "../resources/compiled/iqft/iqft34.qudotc";
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