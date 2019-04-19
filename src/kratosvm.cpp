#include <iostream>
#include <cstdio>
#include "common.h"
#include "quworld.h"

int main(int argc, char *argv[]){
   std::cout << "QuDot Kratos Virtual Machine" << std::endl;

   QuWorld myWorld(5, 1, ONE_AMP64);
   myWorld.setDotAmplitude(2, ZERO, ROOT2);
   myWorld.setDotAmplitude(2, ONE, ROOT2);
   myWorld.setDotAmplitude(5, ZERO, ROOT2);
   myWorld.setDotAmplitude(5, ONE, ROOT2);   
   
   for (int i=0; i < 100; i++) {
      std::cout << myWorld.measure() << std::endl;
   }
   return 0;
}
