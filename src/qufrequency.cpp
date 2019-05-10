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

    long long QuFrequency::getCount(const std::string& value) const {
        tbb::concurrent_hash_map<std::string, long long>::const_accessor a;
        if (_freq_table.find(a, value)) {
            return a->second;
        } else {
            return 0;
        }
    }

    double QuFrequency::getPct(const std::string& value) const {
        if (_sum_table > 0) {
            return getCount(value) / static_cast<double>(_sum_table);
        }
        return 0.0;
    }

    double QuFrequency::getPct(const long long count) const {
        return count / static_cast<double>(_sum_table);
    }

    tbb::concurrent_hash_map<std::string, long long>::iterator QuFrequency::begin() {
        return _freq_table.begin();
    }

    tbb::concurrent_hash_map<std::string, long long>::iterator QuFrequency::end() {
        return _freq_table.end();
    }
}