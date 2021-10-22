#include "qudot/components/eprunit.h"

#include <iostream>
#include <string>

#include <boost/log/trivial.hpp>

#include "tbb/parallel_for.h"
#include "tbb/concurrent_unordered_map.h"
#include "tbb/concurrent_hash_map.h"
#include "tbb/concurrent_unordered_set.h"
#include "tbb/blocked_range.h"

using std::string;
using tbb::concurrent_unordered_map;
using tbb::concurrent_unordered_set;
using tbb::concurrent_hash_map;

typedef concurrent_hash_map<string, concurrent_unordered_set<size_t>> SigMap;
namespace qudot {
    void EPRUnit::mergeWorlds(QuMvN* qumvn, double epsilon) {
        BOOST_LOG_TRIVIAL(info) << "num worlds: " << qumvn->size();
        BOOST_LOG_TRIVIAL(info) << "epsilon: " << epsilon;
        SigMap world_sigs(qumvn->size() / 2);  

        tbb::parallel_for(qumvn->range(), [&](const WorldMap::const_range_type &r) {
            for (auto it = r.begin(); it != r.end(); ++it) {
                QuWorld* quworld = it->second.get();   
                string sig = quworld->getWorldSigniture();
                SigMap::accessor a;    
                if (world_sigs.find(a, sig)) {
                    a->second.insert(quworld->getId());
                    a.release();                    
                } else {
                    world_sigs.insert(a, sig);
                    a->second.insert(quworld->getId());
                    a.release();
                }
            }
        });
        BOOST_LOG_TRIVIAL(info) << world_sigs.size() << " unique world signitures";
        tbb::parallel_for(world_sigs.range(), [&qumvn, &epsilon](const SigMap::const_range_type &r) {
            for (auto it=r.begin(); it != r.end(); ++it) {
                if (it->second.size() > 1) {
                    qumvn->mergeWorlds(it->second, epsilon);
                }    
            }
        });
        BOOST_LOG_TRIVIAL(info) << "done merging worlds";
    }
}