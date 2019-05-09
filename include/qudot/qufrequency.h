#ifndef __QUDOT_QUFREQUENCY_H
#define __QUDOT_QUFREQUENCY_H

#include <string>
#include "tbb/tbb.h"

namespace qudot {
    class QuFrequency {
        private:
            long long _sum_table;
            tbb::concurrent_hash_map<std::string, long long> _freq_table;

        public:
            QuFrequency(long long);
            void addValue(const std::string&);
            long long getCount(const std::string&) const;
            double getPct(const std::string&) const;
            tbb::concurrent_hash_map<std::string, long long>::iterator begin();
            tbb::concurrent_hash_map<std::string, long long>::iterator end(); 
    };
}

#endif