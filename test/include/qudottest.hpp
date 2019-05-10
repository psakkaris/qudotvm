#ifndef __QUDOT_TEST_ABSTRACTTEST_H
#define __QUDOT_TEST_ABSTRACTTEST_H

#include <cstdio>
#include <string>
#include <stdexcept>
#include <map>
#include <memory>

#include "gtest/gtest.h"

#include "qudot/qudot.h"
#include "qudot/qudotconfig.h"
#include "qudot/kratosvm.h"
#include "qudot/qufrequency.h"
#include "qudot/components/heisenbergunit.h"


namespace qudot {
namespace test {
    // K1 is KratosVM1
    enum VMVersion {K1};
    const double PERCENT_DELTA = 5.0;

        std::unique_ptr<QuDot> getVM(std::string& filename, VMVersion vm_version, bool is_bytecode) {
        if (vm_version == K1) {
            QuDotConfig config;
            return std::make_unique<KratosVM>(filename, config);
        } else {
            throw std::runtime_error("Unrecognized VM Option");
        }                   
    }

    bool assertFrequency(QuFrequency& freq, std::map<std::string, double> theory, bool make_assertion) {
        for (auto it=freq.begin(); it != freq.end(); ++it) {
            auto theory_it = theory.find(it->first);
            if (theory_it != theory.end()) {
                if (make_assertion) {
                    printf("%s -> Theory %f, Expiremnt: %f ", it->first.c_str(), theory_it->second, freq.getPct(it->second));
                    
                    double error = HeisenbergUnit::percentError(freq.getPct(it->second), theory_it->second);
                    printf("Percent Error: %f\n", error);
                    if(error > PERCENT_DELTA) {
                        return false;
                    }
                } else {
                    printf("%s -> Expirement: %f\n", it->first.c_str(), freq.getPct(it->second));
                }
            } else {
                printf("%s invalid theoretical state\n", it->first.c_str());
                return false;
            }
        }
        return true;
    }

    bool testAgainstTheory(std::string filename, std::map<std::string, double>& theory, VMVersion vm_version = K1, 
                            double error_threshold = PERCENT_DELTA, bool is_bytecode = true, bool make_assertion = true) 
    {
        std::unique_ptr<QuDot> vm = getVM(filename, vm_version, is_bytecode);
        vm->bohr();
        QuFrequency freq(vm->getEnsemble());
        vm->getResults(freq);

        return assertFrequency(freq, theory, make_assertion);
    }
}
}
#endif