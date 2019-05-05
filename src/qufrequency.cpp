#include "qudot/qufrequency.h"

#include <string>
#include "tbb/tbb.h"

namespace qudot {
    QuFrequency::QuFrequency(long long sum_table) : _sum_table(sum_table) { }

    void QuFrequency::addValue(const std::string& result) {
        tbb::concurrent_hash_map<std::string, long long>::accessor a;
        _freq_table.insert(a, result);
        a->second += 1;
    }

    long long QuFrequency::getCount(const std::string& value) {
        tbb::concurrent_hash_map<std::string, long long>::const_accessor a;
        if (_freq_table.find(a, value)) {
            return a->second;
        } else {
            return 0;
        }
    }

    tbb::concurrent_hash_map<std::string, long long>::iterator QuFrequency::begin() {
        return _freq_table.begin();
    }

    tbb::concurrent_hash_map<std::string, long long>::iterator QuFrequency::end() {
        return _freq_table.end();
    }
}