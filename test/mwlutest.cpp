#include "gtest/gtest.h"
#include "qudottest.h"

TEST(MwluTest, addModN1) {
    std::string filename = "../resources/compiled/mwlu/addModN1.qudotc";
    std::map<std::string, double> theory;

    theory["0111"] = .25;
    theory["0000"] = .25;
    theory["1011"] = .25;
    theory["0100"] = .25;    

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));    
}

TEST(MwluTest, addModN2) {
    std::string filename = "../resources/compiled/mwlu/addModN2.qudotc";
    std::map<std::string, double> theory;

    theory["0010"] = .25;
    theory["0000"] = .25;
    theory["0100"] = .25;
    theory["0110"] = .25;    

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));    
}

TEST(MwluTest, addModN3) {
    std::string filename = "../resources/compiled/mwlu/addModN3.qudotc";
    std::map<std::string, double> theory;

    theory["0001"] = .25;
    theory["1001"] = .25;
    theory["0101"] = .25;
    theory["1101"] = .25;    

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));    
}

TEST(MwluTest, addModN4) {
    std::string filename = "../resources/compiled/mwlu/addModN4.qudotc";
    std::map<std::string, double> theory;

    theory["0101"] = .25;
    theory["1001"] = .25;
    theory["1101"] = .25;
    theory["0010"] = .25;    

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));    
}

TEST(MwluTest, addModN5) {
    std::string filename = "../resources/compiled/mwlu/addModN5.qudotc";
    std::map<std::string, double> theory;

    theory["0011"] = .25;
    theory["0101"] = .213;
    theory["0111"] = .125;
    theory["1001"] = .0366;
    theory["1101"] = .0366;
    theory["0000"] = .125;
    theory["0010"] = .213;  

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));    
}

TEST(MwluTest, addModN6) {
    std::string filename = "../resources/compiled/mwlu/addModN6.qudotc";
    std::map<std::string, double> theory;

    theory["10011"] = 1.0;

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));    
}

TEST(MwluTest, ctrlAddModN2) {
    std::string filename = "../resources/compiled/mwlu/ctrlAddModN2.qudotc";
    std::map<std::string, double> theory;

    theory["00010001"] = 1.0;

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));    
}

TEST(MwluTest, ctrlAddModN3) {
    std::string filename = "../resources/compiled/mwlu/ctrlAddModN3.qudotc";
    std::map<std::string, double> theory;

    theory["00010000"] = 1.0;

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));    
}