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
    // K1 is KratosVM1
    enum VMVersion {K1};
    const double PERCENT_DELTA = 5.0;

    KratosVM* getVM(std::string& filename, VMVersion vm_version, bool is_bytecode);

    bool assertFrequency(QuFrequency& freq, std::map<std::string, double>& theory, bool make_assertion);

    bool testAgainstTheory(std::string filename, std::map<std::string, double>& theory, VMVersion vm_version = K1, 
                            double error_threshold = PERCENT_DELTA, bool is_bytecode = true, bool make_assertion = true);
}
#endif