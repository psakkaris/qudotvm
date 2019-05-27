 #include "qudottest.h"   

 #include <iostream>

namespace qudot {    
    KratosVM* getVM(std::string& filename, VMVersion vm_version, bool is_bytecode) {
        if (vm_version == K1) {
            QuDotConfig config;
            return new KratosVM(filename, config);
        } else {
            throw std::runtime_error("Unrecognized VM Option");
        }                   
    }

    bool assertFrequency(QuFrequency& freq, std::map<std::string, double>& theory, bool make_assertion, double threshold) {
        
        // std::cout << "size: " << theory.size() << std::endl;
        // for (auto myit = theory.begin(); myit != theory.end(); ++myit) {
        //     std::cout << myit->first << ":" << myit->second << std::endl;    
        // }
        printf("checking freq\n");
        bool result = true;
        for (auto it=freq.begin(); it != freq.end(); ++it) {

            auto theory_it = theory.find(it->first);
            if (theory_it != theory.end()) {
                if (make_assertion) {
                    printf("%s -> Theory %f, Expiremnt: %f ", it->first.c_str(), theory_it->second, freq.getPct(it->second));
                    
                    double error = HeisenbergUnit::percentError(freq.getPct(it->second), theory_it->second);
                    printf("Percent Error: %f\n", error);
                    if(error > threshold) {
                        result = false;
                    }
                } else {
                    printf("%s -> Expirement: %f\n", it->first.c_str(), freq.getPct(it->second));
                }
            } else {
                printf("%s invalid theoretical state\n", it->first.c_str());
                result = false;
            }
        }
        return result;
    }

    bool testAgainstTheory(std::string filename, std::map<std::string, double>& theory, VMVersion vm_version, 
                            double error_threshold, bool is_bytecode, bool make_assertion) 
    {
        QuDotConfig config;
        KratosVM vm(filename, config);
        vm.bohr();
        QuFrequency freq(vm.getEnsemble());
        vm.getResults(freq);

        return assertFrequency(freq, theory, make_assertion, error_threshold);
    }
}