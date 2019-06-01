#include "qudot/components/heisenbergunit.h"

#include <cmath>
#include <fstream>
#include "tbb/parallel_for.h"

#include "mkl_vsl.h"

namespace qudot {
    void HeisenbergUnit::getResults(QuMvN* qumvn, unsigned int ensemble, QuFrequency& freq) {
        tbb::parallel_for(size_t(0), size_t(ensemble), [&](size_t i) { 
            freq.addValue(qumvn->measure()); 
        });
    }

    void HeisenbergUnit::printResults(std::ostream& out, QuFrequency& freq) {
        out << "Value \t Freq. \t Pct. \n";
        for (auto it = freq.begin(); it != freq.end(); ++it) {
            out << it->first << "\t" << it->second << "\t" << freq.getPct(it->second) << "\n";    
        }    
    }

    void HeisenbergUnit::saveResults(const std::string& filename, QuFrequency& freq) {
        std::ofstream myfile;
        myfile.open(filename);
        myfile << "state,frequency,pct\n";    
        for (auto it = freq.begin(); it != freq.end(); ++it) {
            myfile << it->first << "," << it->second << "," << freq.getPct(it->second) << "\n";    
        }
    }

    double HeisenbergUnit::percentError(const double experiment, const double theory) {
        double diff = experiment - theory;
        double ratio = diff / theory;
        return std::abs(ratio) * 100.0;        
    }
}