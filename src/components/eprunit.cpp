#include "qudot/components/eprunit.h"

#include <iostream>
#include <string>

#include "tbb/parallel_for.h"
#include "tbb/concurrent_unordered_map.h"
#include "tbb/concurrent_unordered_set.h"

using std::string;
using tbb::concurrent_unordered_map;
using tbb::concurrent_unordered_set;

namespace qudot {
    void EPRUnit::mergeWorlds(QuMvN* qumvn, double epsilon) {
        std::cout << "num worlds: " << qumvn->size() << "\n";
        std::cout << "epsilon: " << epsilon << "\n";
        concurrent_unordered_map<string, concurrent_unordered_set<size_t>> world_sigs(qumvn->size() / 2);  

        tbb::parallel_for(qumvn->range(), [&](const WorldMap::const_range_type &r) {
            for (auto it = r.begin(); it != r.end(); ++it) {
                QuWorld* quworld = it->second.get();   
                string sig = quworld->getWorldSigniture();    
                world_sigs[sig].insert(quworld->getId()); 
            }
        });
        std::cout << world_sigs.size() << " unique world signitures\n";
        tbb::parallel_for(world_sigs.range(), [&](const concurrent_unordered_map<string, concurrent_unordered_set<size_t>>::const_range_type &r) {
            for (auto it=r.begin(); it != r.end(); ++it) {
                if (it->second.size() > 1) {
                    qumvn->mergeWorlds(it->second, epsilon);
                }    
            }
        });
        std::cout << "worlds merged\n";
    }
}