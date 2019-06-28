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
#include "qudot/components/heisenbergunit.h"


std::string getOutFilename(std::string& in_filename) {
   auto found = in_filename.find_first_of('.');
   if (found != std::string::npos) {
      return in_filename.substr(0, found) + ".out";
   } else {
      return in_filename + ".out";
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
       } else if (arg.compare("-qupw") == 0) {
          config.setPrintWorlds(true);
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
      if (config.getPrintResults()) {
         qudot::HeisenbergUnit::printResults(std::cout, freq);
      }
      std::cout << "saving results to file\n";
      std::string out_filename = getOutFilename(filename);
      qudot::HeisenbergUnit::saveResults(out_filename, freq);

      if (config.getPrintWorlds()) {
         vm.printWorlds(out_filename + "worlds");
      }
   } catch(std::runtime_error &re) {
      std::cerr << "error: " << re.what() << std::endl;
      return 1;   
   }

   return 0;
}
