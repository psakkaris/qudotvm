#include "gtest/gtest.h"

#include "qudottest.hpp"

TEST(BasicMeasurementTest, state2test) {
    std::string filename = "../resources/compiled/measurements/state2.qudotc";
    std::map<std::string, double> theory = { {"00100", 1.0} };

    ASSERT_TRUE(qudot::test::testAgainstTheory(filename, theory));
}

TEST(BasicMeasurementTest, state3test) {
    std::string filename = "../resources/compiled/measurements/state3.qudotc";   
    std::map<std::string, double> theory2 = { {"00000", 0.031249999999999993}, {"00001", 0.031249999999999993} };
    ASSERT_TRUE(qudot::test::testAgainstTheory(filename, theory2)); 
}

/*
TEST(BasicMeasurementTest, state3test) {
    std::string filename = "../resources/compiled/measurements/state4.qudotc";   

    qudot::test::testAgainstTheory(filename, theory); 
}

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