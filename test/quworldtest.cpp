#include <string>

#include "gtest/gtest.h"
#include "quworld.h"

TEST(QuWorldTest, testCreate) {
    QuAmp oneAmp = QuAmp(1,0);
    QuAmp64 oneAmp64 = QuAmp64(1,0);
    QuAmp zeroAmp = QuAmp(0,0);
    QuAmp64 zeroAmp64 = QuAmp64(0,0);
    QuAmp iAmp = QuAmp(0,1);
    QuAmp64 iAmp64 = QuAmp64(0,1);

    QuWorld* quworld = new QuWorld(5, 0, oneAmp64);
    ASSERT_EQ(quworld->getId(), 0);
    ASSERT_EQ(quworld->getNumQubits(), 5);
    ASSERT_EQ(quworld->getWorldAmplitude(), QuAmp64(1,0));
    ASSERT_NE(quworld->getWorldAmplitude(), zeroAmp64);
    quworld->setWorldAmplitude(iAmp64);

    ASSERT_EQ(quworld->getWorldAmplitude(), iAmp64);
    ASSERT_EQ(quworld->getEnablingQubit(), true);
    quworld->setEnablingQubit(false);
    ASSERT_FALSE(quworld->getEnablingQubit());
    delete quworld;
}

TEST(QuWorldTest, testQuDotNet) {
    QuWorld myWorld(5, 1, QuAmp64(1,0));
    ASSERT_TRUE(myWorld.isActive(1, ZERO));
    ASSERT_TRUE(myWorld.isActive(2, ZERO));
    ASSERT_TRUE(myWorld.isActive(3, ZERO));
    ASSERT_TRUE(myWorld.isActive(4, ZERO));
    ASSERT_TRUE(myWorld.isActive(5, ZERO));

    ASSERT_FALSE(myWorld.isActive(1, ONE));
    ASSERT_FALSE(myWorld.isActive(2, ONE));
    ASSERT_FALSE(myWorld.isActive(3, ONE));
    ASSERT_FALSE(myWorld.isActive(4, ONE));
    ASSERT_FALSE(myWorld.isActive(5, ONE));

    int qubits1[3] = {1, 3, 5};
    ASSERT_TRUE(myWorld.areActive(qubits1, 3, ZERO));
    ASSERT_FALSE(myWorld.areActive(qubits1, 3, ONE));

    int qubits2[5] = {1,2,3,4};
    ASSERT_TRUE(myWorld.areActive(qubits2, 4, ZERO));
    ASSERT_FALSE(myWorld.areActive(qubits2, 4, ONE));
}

TEST(QuWorldTest, testAmplitudes) {
    QuWorld myWorld(5, 1, ONE_AMP64);
    ASSERT_EQ(myWorld.getOneAmplitude(3), ZERO_AMP);
    ASSERT_EQ(myWorld.getOneAmplitude(1), ZERO_AMP);
    ASSERT_EQ(myWorld.getOneAmplitude(5), ZERO_AMP);
    ASSERT_EQ(myWorld.getZeroAmplitude(3), ONE_AMP);
    ASSERT_EQ(myWorld.getZeroAmplitude(4), ONE_AMP);
    ASSERT_EQ(myWorld.getZeroAmplitude(5), ONE_AMP);
    ASSERT_NE(myWorld.getZeroAmplitude(1), ZERO_AMP);
}

TEST(QuWorldTest, testProbability) {
    QuWorld myWorld(3, 1, ONE_AMP64);
    ASSERT_EQ(myWorld.getQubitProbability(1, ZERO), 1);
    ASSERT_EQ(myWorld.getQubitProbability(3, ONE), 0);

    myWorld.setDotAmplitude(2, ZERO, ROOT2);
    myWorld.setDotAmplitude(2, ONE, ROOT2);

    ASSERT_FLOAT_EQ(.5, myWorld.getQubitProbability(2, ZERO));
    ASSERT_FLOAT_EQ(.5, myWorld.getQubitProbability(2, ONE));
    ASSERT_EQ(myWorld.getQubitProbability(1, ZERO), 1);
    ASSERT_EQ(myWorld.getQubitProbability(1, ONE), 0);
}

TEST(QuWorldTest, testSplitWorlds) {
    QuWorld myWorld(3, 1, ONE_AMP64);
    myWorld.setDotAmplitude(2, ZERO, ROOT2);
    myWorld.setDotAmplitude(2, ONE, ROOT2);    

    ASSERT_TRUE(myWorld.isSplitWorlds(2));
    ASSERT_FALSE(myWorld.isSplitWorlds(1));
    ASSERT_FALSE(myWorld.isSplitWorlds(3));

    myWorld.setDotAmplitude(3, ZERO, ROOT2);
    myWorld.setDotAmplitude(3, ONE, ROOT2);   
    ASSERT_TRUE(myWorld.isSplitWorlds(3));
}

TEST(QuWorldTest, testActivation) {
    QuWorld myWorld(5, 1, ONE_AMP64);
    myWorld.setDotAmplitude(2, ZERO, ROOT2);
    myWorld.setDotAmplitude(2, ONE, ROOT2);
    myWorld.setDotAmplitude(3, ZERO, ROOT2);
    myWorld.setDotAmplitude(3, ONE, ROOT2);
    myWorld.setDotAmplitude(4, ZERO, ROOT2);
    myWorld.setDotAmplitude(4, ONE, ROOT2);    

    ASSERT_TRUE(myWorld.isSplitWorlds(4));
    myWorld.deactivateChildren(3, ONE);
    ASSERT_TRUE(myWorld.isSplitWorlds(4));
    ASSERT_FLOAT_EQ(myWorld.getQubitProbability(4, ZERO), .5);
    ASSERT_FLOAT_EQ(myWorld.getQubitProbability(4, ONE), .5);

    myWorld.deactivate(2, ZERO);
    ASSERT_FALSE(myWorld.isSplitWorlds(2));
    myWorld.deactivate(3, ONE);
    ASSERT_FALSE(myWorld.isSplitWorlds(3));

    // should only activate if not (0,0)
    myWorld.activate(2, ZERO);
    ASSERT_FALSE(myWorld.isSplitWorlds(2));
}

TEST(QuWorldTest, testNetEquality) {
    QuWorld myWorld(5, 1, ONE_AMP64);
    myWorld.setDotAmplitude(2, ZERO, ROOT2);
    myWorld.setDotAmplitude(2, ONE, ROOT2);
    myWorld.setDotAmplitude(3, ZERO, ROOT2);
    myWorld.setDotAmplitude(3, ONE, ROOT2);
    myWorld.setDotAmplitude(4, ZERO, ROOT2);
    myWorld.setDotAmplitude(4, ONE, ROOT2);  

    QuWorld myWorld2(5, 2, ONE_AMP64);

    ASSERT_FALSE(myWorld.areNetsEqual(myWorld2));    

    myWorld2.setDotAmplitude(2, ZERO, ROOT2);
    myWorld2.setDotAmplitude(2, ONE, ROOT2);
    myWorld2.setDotAmplitude(3, ZERO, ROOT2);
    myWorld2.setDotAmplitude(3, ONE, ROOT2);
    myWorld2.setDotAmplitude(4, ZERO, ROOT2);
    myWorld2.setDotAmplitude(4, ONE, ROOT2);  

    ASSERT_TRUE(myWorld2.areNetsEqual(myWorld));

    QuWorld myWorld3 = QuWorld(3, 3, ONE_AMP64);

    ASSERT_FALSE(myWorld.areNetsEqual(myWorld3));
    ASSERT_FALSE(myWorld2.areNetsEqual(myWorld3));
}

TEST(QuWorldTest, testMeasure) {
   QuWorld myWorld(3, 1, ONE_AMP64);
   myWorld.setDotAmplitude(2, ZERO, ROOT2);
   myWorld.setDotAmplitude(2, ONE, ROOT2);

   std::string results[10];
   for (int i = 0; i < 10; i++) {
       results[i] = myWorld.measure();
       ASSERT_TRUE(results[i] == "000" || results[i] == "010"); 
   }    

   ASSERT_NE(myWorld.measure(), "111");
}