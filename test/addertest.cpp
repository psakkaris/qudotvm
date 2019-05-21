#include "gtest/gtest.h"
#include "qudottest.h"

TEST(FullAdderTest, adder1) {
    std::string filename = "../resources/compiled/adder/fullAdder1.qudotc";
    std::map<std::string, double> theory;

    theory["0001"] = .25;
    theory["0101"] = .25;
    theory["1001"] = .25;
    theory["1101"] = .25;    

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));
}

TEST(FullAdderTest, adder2) {
    std::string filename = "../resources/compiled/adder/fullAdder2.qudotc";
    std::map<std::string, double> theory;

    theory["0111"] = .25;
    theory["1011"] = .25;
    theory["1111"] = .25;
    theory["0011"] = .25;    

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));
}

TEST(FullAdderTest, adder3) {
    std::string filename = "../resources/compiled/adder/fullAdder3.qudotc";
    std::map<std::string, double> theory;
    //00000,24744,0.123720
    theory["00111"] = .124;
    //00010,21425,0.107125
    theory["01001"] = .107;
    //00100,12580,0.062900
    theory["01011"] = .063;
    //00110,3600,0.018000
    theory["01101"] = .018;
    //01010,3624,0.018120
    theory["10001"] = .018;
    //01100,12610,0.063050
    theory["10011"] = .063;
    //01110,21311,0.106555
    theory["10101"] = .107;
    //10000,24868,0.124340
    theory["10111"] = .124;
    //10010,21433,0.107165
    theory["11001"] = .107;
    //10100,12500,0.062500
    theory["11011"] = .063;
    //10110,3763,0.018815
    theory["11101"] = .018;
    //11010,3629,0.018145
    theory["00001"] = .018;
    //11100,12569,0.062845
    theory["00011"] = .063;
    //11110,21344,0.106720
    theory["00101"] = .107;  

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));
}

TEST(FullAdderTest, adder4) {
    std::string filename = "../resources/compiled/adder/fullAdder4.qudotc";
    std::map<std::string, double> theory;

    theory["0011"] = .25;
    theory["0101"] = .213;
    theory["0111"] = .125;
    theory["1001"] = .0366;
    theory["1101"] = .0366;
    theory["1111"] = .125;
    theory["0001"] = .213;   

    EXPECT_TRUE(qudot::testAgainstTheory(filename, theory));
}