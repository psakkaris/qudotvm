 #include "qudottest.h"   

#include <cmath>
#include <iostream>

#include <boost/multiprecision/cpp_int.hpp>

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

    int getEvenPeriod(const int result, const int tm, const int N) {
        // if the observed value is 0 we cannot get the period
        if (result == 0) return -1;
        // if the observe value is a power of 2 therefore a factor of tm then the period is easy, otherwise
        // we have to do a continued fraction expansion
        if (tm % result == 0) {
            return tm / result;
        }  
        // continued fraction expansion
        Cf cf(result, tm);
        int p = 0;
        int q = 0;
        for (int i=2; i < MAX_CF_IT; i++) {
            int pi = cf.getP(i);
            int qi = cf.getQ(i);
            if ((pi == 0 && qi == 0) || qi >= N) {
                break;
            }
            p = pi;
            q = qi;
            printf("%d -> %d: %d/%d\n", result, i, pi, qi);
        }
        printf("Convergent: %d/%d\n", p, q);

        if ( q % 2 == 0) {
            return q;
        } else {
            return -1;
        }             
    }

    std::pair<int, int> factor(KratosVM& vm, const int N, const int x, const int k, const int l, const int tm) {
        std::string result = vm.measure();
        result.erase(k, l);
        int int_result = std::stoi(result, nullptr, 2);        
        int period = getEvenPeriod(int_result, tm, N);
        int num_runs = 1;
        while (period <= 0 && num_runs < 20) {
            std::string result = vm.measure();
            result.erase(k, l);
            int int_result = std::stoi(result, nullptr, 2);            
            period = getEvenPeriod(int_result, tm, N);
            num_runs++;
        }

        if (period <= 0) {
            printf("No Period Found\n");
            return std::make_pair(1,1);
        }
        printf("Period found: %d\n", period);

        boost::multiprecision::cpp_int mpx(x);
        //boost::multiprecision::cpp_int mpp(period / 2);
        boost::multiprecision::cpp_int mpN(N);
        boost::multiprecision::cpp_int pow_val = boost::multiprecision::pow(mpx, period / 2);
        int factor1 = boost::multiprecision::gcd(pow_val + 1, N).convert_to<int>();
        int factor2 = boost::multiprecision::gcd(pow_val - 1, N).convert_to<int>();
        if (factor1 != 1 && factor1 != N) {
                factor2 = N / factor1;
                return std::make_pair(factor1, factor2);
        } else if (factor2 != 1 && factor2 != N) {
            factor1 = N / factor2;
            return std::make_pair(factor1, factor2);
        } 
        return std::make_pair(0, 0);
    }

    bool shorTest(std::string filename, const int N, const int x, const int k, const int l) {
        QuDotConfig config;
        KratosVM vm(filename, config);
        vm.bohr();
        int tm = std::pow(2, k);     
        int num_measurements = 1;
        bool factored = false;
        do {
            auto factors = factor(vm, N, x, k, l, tm); 
            factored = N == (factors.first * factors.second);
            if (factored) {
                printf("%d = %d x %d\n", N, factors.first, factors.second);
                return true;
            }
            num_measurements++;
        } while (!factored && num_measurements <= 10);

        return false;
    }



 // Cf class

 Cf::Cf(const int val, const int tm) : val(val), tm(tm) {
            a0 = (int) std::floor((double) val/tm);
            e0 = ((double)val/tm) - a0;
 }

int Cf::getP(const int n) {
    if (n == 0) {
        return a0;
    } else if (n == 1) {
        return (getA(e0)*a0) + 1;
    } else {
        int aN = getAn(n);
        if (aN == 0) return -1;

        return (aN * getP(n-1)) + getP(n-2);
    }
}
int Cf::getQ(const int n) {
    if (n==0) {
        return 1;
    } else if (n==1) {
        return getA(e0);
    } else {
        int aN = getAn(n);
        if (aN == 0) return -1;

        return (aN * getQ(n-1)) + getQ(n-2);
    }
}

int Cf::getA(const double epsilon) {
    return (int) std::floor(1.0 / epsilon);
}

double Cf::getE(double espilonN, int aN) {
    return (1.0/espilonN) - aN;
}

int Cf::getAn(const int n) {
    int a = a0;
    double e = e0;

    for (int i=1; i<=n; i++) {
        if (e <= 0) return 0;

        a = getA(e);
        double tempE = e;
        e = getE(tempE, a);
    }
    return a;
}

}