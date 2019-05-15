#include "gtest/gtest.h"

#include "qudottest.h"

#include <memory>
#include <map>
#include "qudot/qudot.h"

TEST(CoherentMeasurementTest, cnotState1) {
    std::string filename = "../resources/compiled/coherent/cnotState1.qudotc";
    std::map<std::string, double> theory;
    theory["00"] = .25;
    theory["01"] = .25;
    theory["10"] = .25;
    theory["11"] = .25;

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));
}

TEST(CoherentMeasurementTest, cnotState2) {
    std::string filename = "../resources/compiled/coherent/cnotState2.qudotc";
    std::map<std::string, double> theory;
    theory["00"] = .5;
    theory["11"] = .5;

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));
}

TEST(CoherentMeasurementTest, semiCnotState3) {
    std::string filename = "../resources/compiled/coherent/semiCnotState3.qudotc";
    std::map<std::string, double> theory;
    theory["01001"] = 0.125;
    theory["10000"] = 0.125;
    theory["11001"] = 0.125;
    theory["00010"] = 0.125;
    theory["11011"] = 0.125;
    theory["01011"] = 0.125;
    theory["00000"] = 0.125;
    theory["10010"] = 0.125;

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));
}

TEST(CoherentMeasurementTest, crotState1) {
    std::string filename = "../resources/compiled/coherent/crotState1.qudotc";
    std::map<std::string, double> theory;
    theory["00000"] = 0.125;
    theory["00001"] = 0.125;
    theory["00010"] = 0.125;
    theory["00011"] = 0.125;
    theory["10000"] = 0.125;
    theory["10001"] = 0.125;
    theory["10010"] = 0.125;
    theory["10011"] = 0.125;

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));
}

TEST(CoherentMeasurementTest, semiCrotState1) {
    std::string filename = "../resources/compiled/coherent/semiCrotState1.qudotc";
    std::map<std::string, double> theory;
    theory["00000"] = 0.125;
    theory["00001"] = 0.125;
    theory["00010"] = 0.125;
    theory["00011"] = 0.125;
    theory["10000"] = 0.125;
    theory["10001"] = 0.125;
    theory["10010"] = 0.125;
    theory["10011"] = 0.125;

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));
}