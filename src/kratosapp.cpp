#include <iostream>
#include <cstdio>
#include <stdexcept>
#include <string>

#include "qudot/common.h"
#include "qudot/kratosvm.h"
#include "qudot/qufrequency.h"
#include "qudot/qureg.h"
#include "qudot/quworld.h"
#include "qudot/qudotconfig.h"


void doSomeQuantum() {
   qudot::QuWorld myWorld(5, 1, qudot::ONE_AMP64);
   myWorld.setDotAmplitude(2, qudot::ZERO, qudot::ROOT2);
   myWorld.setDotAmplitude(2, qudot::ONE, qudot::ROOT2);
   myWorld.setDotAmplitude(5, qudot::ZERO, qudot::ROOT2);
   myWorld.setDotAmplitude(5, qudot::ONE, qudot::ROOT2);   
   
   myWorld.swapQubits(2,4);
   myWorld.swapQubits(5,1);
   for (int i=0; i < 10; i++) {
      std::cout << myWorld.measure() << std::endl;
   }

   qudot::QuReg reg(5, 8);
   printf("qureg: %d\n", reg.getSize());
   for (auto i=reg.getQubits().begin(); i != reg.getQubits().end(); i++) {
      printf("%d\n", *i);
   }

}

void printQuFrequency(qudot::QuFrequency& freq) {
   for (auto it = freq.begin(); it != freq.end(); ++it) {
      printf("%s\t%11u\n", it->first.c_str(), it->second);
   }
}

int main(int argc, char *argv[]) {
   std::cout << "QuDot Kratos Virtual Machine |001>" << std::endl;

   qudot::QuDotConfig config;
   int arg_ptr = 1;
   std::string filename;
   while (arg_ptr < argc) {
       std::string arg = std::string(argv[arg_ptr]);
       if (arg.compare("-qubits") == 0) {
          arg_ptr++;
          config.setNumQubits(std::stoi(std::string(argv[arg_ptr])));     
       } else if (arg.compare("-stack") == 0) {
          arg_ptr++;
          config.setStackSize(std::stoi(std::string(argv[arg_ptr])));
       } else if (arg.compare("-ensemble") == 0) {
          arg_ptr++;
          config.setEnsembleSize(std::stoi(std::string(argv[arg_ptr])));
       } else if (arg.compare("-print") == 0) {
          config.setPrintResults(true);
       } else if (arg.compare("-multiverse") == 0) {
          arg_ptr++;
          config.setMultiverseSize(std::stoi(std::string(argv[arg_ptr])));
       } else {
          filename = std::string(argv[arg_ptr]);
       }
       arg_ptr++;
   }

   if (filename.size() == 0) {
      std::cerr << "no input file found" << std::endl;
      return 1;
   }

   try {
      qudot::KratosVM vm(filename, config);
      vm.bohr();
      qudot::QuFrequency freq(vm.getEnsemble());
      vm.getResults(freq);
      printQuFrequency(freq);
   } catch(std::runtime_error &re) {
      std::cerr << re.what() << std::endl;
      return 1;   
   }

   return 0;
}
