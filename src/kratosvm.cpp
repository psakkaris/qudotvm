#include <iostream>
#include <cstdio>
#include "common.h"
#include "quworld.h"

void doSomeQuantum() {
   QuWorld myWorld(5, 1, ONE_AMP64);
   myWorld.setDotAmplitude(2, ZERO, ROOT2);
   myWorld.setDotAmplitude(2, ONE, ROOT2);
   myWorld.setDotAmplitude(5, ZERO, ROOT2);
   myWorld.setDotAmplitude(5, ONE, ROOT2);   
   
   myWorld.swapQubits(2,4);
   myWorld.swapQubits(5,1);
   for (int i=0; i < 10; i++) {
      std::cout << myWorld.measure() << std::endl;
   }
}

int main(int argc, char *argv[]){
   std::cout << "QuDot Kratos Virtual Machine" << std::endl;

   doSomeQuantum();
   return 0;
}
