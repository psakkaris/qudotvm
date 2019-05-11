#include "gtest/gtest.h"

#include "qudottest.h"

#include <memory>
#include <map>
#include "qudot/qudot.h"

TEST(BasicMeasurementTest, state2test) {
    std::string filename = "../resources/compiled/measurements/state2.qudotc";
    std::map<std::string, double> theory = { {"00100", 1.0} };

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));
}

TEST(BasicMeasurementTest, state3test) {
    std::string filename = "../resources/compiled/measurements/state3.qudotc";

    std::map<std::string, double> theory2;
    theory2["00000"] = 0.031249999999999993;
    theory2["00001"] = 0.031249999999999993;
    theory2["00010"] = 0.031249999999999993;
    theory2["00011"] = 0.031249999999999993;
    theory2["00100"] = 0.031249999999999993;
    theory2["00101"] = 0.031249999999999993; 
    theory2["00110"] = 0.031249999999999993;
    theory2["00111"] = 0.031249999999999993;
    theory2["01000"] = 0.031249999999999993;
    theory2["01001"] = 0.031249999999999993;
    theory2["01010"] = 0.031249999999999993; 
    theory2["01011"] = 0.031249999999999993;
    theory2["01100"] = 0.031249999999999993;
    theory2["01101"] = 0.031249999999999993;
    theory2["01110"] = 0.031249999999999993;
    theory2["01111"] = 0.031249999999999993; 
    theory2["10000"] = 0.031249999999999993; 
    theory2["10001"] = 0.031249999999999993;
    theory2["10010"] = 0.031249999999999993;
    theory2["10011"] = 0.031249999999999993;
    theory2["10100"] = 0.031249999999999993;
    theory2["10101"] = 0.031249999999999993; 
    theory2["10110"] = 0.031249999999999993;
    theory2["10111"] = 0.031249999999999993;
    theory2["11000"] = 0.031249999999999993;
    theory2["11001"] = 0.031249999999999993;
    theory2["11010"] = 0.031249999999999993; 
    theory2["11011"] = 0.031249999999999993;
    theory2["11100"] = 0.031249999999999993;
    theory2["11101"] = 0.031249999999999993;
    theory2["11110"] = 0.031249999999999993;
    theory2["11111"] = 0.031249999999999993;   

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory2));
}


// TEST(BasicMeasurementTest, state4test) {
//     std::string filename = "../resources/compiled/measurements/state4.qudotc";

//     std::map<std::string, double> theory;
//     theory["10000"] = .5;
//     theory["10001"] = .5;

//     qudot::QuDotConfig config;
//     qudot::KratosVM vm(filename, config);
//     qudot::QuFrequency freq(vm.getEnsemble());
//     vm.bohr();
//     vm.getResults(freq);

//     EXPECT_TRUE(qudot::test::assertFrequency(freq, theory, true)); 
// }

/*
TEST(BasicMeasurementTest, state3test) {
    std::string filename = "../resources/compiled/measurements/state5.qudotc";   

    qudot::test::testAgainstTheory(filename, theory); 
}

TEST(BasicMeasurementTest, state3test) {
    std::string filename = "../resources/compiled/measurements/state6.qudotc";   

    qudot::test::testAgainstTheory(filename, theory); 
}

TEST(BasicMeasurementTest, state3test) {
    std::string filename = "../resources/compiled/measurements/state3.qudotc";   

    qudot::test::testAgainstTheory(filename, theory); 
}

TEST(BasicMeasurementTest, state3test) {
    std::string filename = "../resources/compiled/measurements/state7.qudotc";   

    qudot::test::testAgainstTheory(filename, theory); 
}

TEST(BasicMeasurementTest, state3test) {
    std::string filename = "../resources/compiled/measurements/state8.qudotc";   

    qudot::test::testAgainstTheory(filename, theory); 
}

TEST(BasicMeasurementTest, state3test) {
    std::string filename = "../resources/compiled/measurements/state9.qudotc";   

    qudot::test::testAgainstTheory(filename, theory); 
}

TEST(BasicMeasurementTest, state3test) {
    std::string filename = "../resources/compiled/measurements/state10.qudotc";   

    qudot::test::testAgainstTheory(filename, theory); 
}
*/