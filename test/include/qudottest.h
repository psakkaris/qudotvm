#ifndef __QUDOT_TEST_ABSTRACTTEST_H
#define __QUDOT_TEST_ABSTRACTTEST_H

#include <cstdio>
#include <string>
#include <stdexcept>
#include <map>
#include <memory>
#include <utility>

#include "gtest/gtest.h"
#include "mkl_vsl.h"

#include "qudot/qudot.h"
#include "qudot/qudotconfig.h"
#include "qudot/kratosvm.h"
#include "qudot/qufrequency.h"
#include "qudot/components/heisenbergunit.h"


namespace qudot {
    // K1 is KratosVM1
    enum VMVersion {K1};
    const double PERCENT_DELTA = 5.0;
    const int MAX_CF_IT = 10;

    KratosVM* getVM(std::string& filename, VMVersion vm_version, bool is_bytecode);

    bool assertFrequency(QuFrequency& freq, std::map<std::string, double>& theory, bool make_assertion, double threshold = PERCENT_DELTA);

    bool testAgainstTheory(std::string filename, std::map<std::string, double>& theory, VMVersion vm_version = K1, 
                            double error_threshold = PERCENT_DELTA, bool is_bytecode = true, bool make_assertion = true);

    std::pair<int, int> factor(KratosVM& vm, const int N, const int x, const int k, const int l, const int tm);

    int getEvenPeriod(const int result, const int tm, const int N);                        

    bool shorTest(std::string filename, const int N, const int x, const int k, const int l);    

    void getRand(double dest[], int num_rand);                    

    class Cf {
        private:
            int val;
            // 2^upper_register
            int tm;
            int a0;
            double e0;
        public:
            Cf(const int val, const int tm);
            int getP(const int n);
            int getQ(const int n);
            int getA(const double epsilon);
            double getE(const double epsilon, const int aN);
            int getAn(const int n);
    };
}
#endif