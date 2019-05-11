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


TEST(BasicMeasurementTest, state4test) {
    std::string filename = "../resources/compiled/measurements/state4.qudotc";
    std::map<std::string, double> theory;
    theory["10000"] = .5;
    theory["10001"] = .5;

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));
}

TEST(BasicMeasurementTest, state5test) {
    std::string filename = "../resources/compiled/measurements/state5.qudotc";
    std::map<std::string, double> theory;    
    theory["00010"] = 0.25;
    theory["00000"] = 0.25;
    theory["10000"] = 0.25;
    theory["10010"] = 0.25;

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));
}

TEST(BasicMeasurementTest, state6test) {
    std::string filename = "../resources/compiled/measurements/state6.qudotc";
    std::map<std::string, double> theory;    
    theory["01001"] = 0.125;
    theory["10001"] = 0.125;
    theory["11001"] = 0.125;
    theory["00001"] = 0.125;
    theory["01101"] = 0.125;
    theory["00101"] = 0.125;
    theory["10101"] = 0.125;
    theory["11101"] = 0.125;

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));
}


TEST(BasicMeasurementTest, state7test) {
    std::string filename = "../resources/compiled/measurements/state7.qudotc";   
    std::map<std::string, double> theory; 
    theory["01011"] = 1.0;

    qudot::testAgainstTheory(filename, theory); 
}

TEST(BasicMeasurementTest, state8test) {
    std::string filename = "../resources/compiled/measurements/state8.qudotc";
    std::map<std::string, double> theory;    
    theory["00100"] = 0.25;
    theory["00110"] = 0.25;
    theory["10100"] = 0.25;
    theory["10110"] = 0.25;

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));
}

TEST(BasicMeasurementTest, state9test) {
    std::string filename = "../resources/compiled/measurements/state9.qudotc";
    std::map<std::string, double> theory;    
    theory["00100"] = 0.25;
    theory["00110"] = 0.25;
    theory["10100"] = 0.25;
    theory["10110"] = 0.25;

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));
}

TEST(BasicMeasurementTest, state10test) {
    std::string filename = "../resources/compiled/measurements/state10.qudotc";
    std::map<std::string, double> theory;    
    theory["01101"] = 0.25;
    theory["01111"] = 0.25;
    theory["11101"] = 0.25;
    theory["11111"] = 0.25;

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));
}
