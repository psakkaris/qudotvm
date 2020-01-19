#include "qudot/components/eprunit.h"
#include "qudot/components/thanosunit.h"

#include <iostream>
#include <map>
#include <string>

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
        std::cout << "num worlds: " << qumvn->size() << "\n";
        std::cout << "epsilon: " << epsilon << "\n";
        SigMap world_sigs(qumvn->size() / 2);  
        ThanosUnit thanos;

        tbb::parallel_for(qumvn->range(), [&](const WorldMap::const_range_type &r) {
            for (auto it = r.begin(); it != r.end(); ++it) {
                QuWorld* quworld = it->second.get();   
                string sig = thanos.getQuDna(quworld);
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
        std::cout << world_sigs.size() << " unique world signitures\n";
        tbb::parallel_for(world_sigs.range(), [&qumvn, &epsilon](const SigMap::const_range_type &r) {
            //std::cout << "YOYOYO\n";
            for (auto it=r.begin(); it != r.end(); ++it) {
                if (it->second.size() > 1) {
                    qumvn->mergeWorlds(it->second, epsilon);
                }    
            }
        });
        std::cout << "identical worlds merged\n";
        
        // int num_snaps = 0;
        // for (auto it=qumvn->begin(); it != qumvn->end(); ++it) {
        //     auto it2 = it;
        //     ++it2;
        //     for (; it2 != qumvn->end(); ++it2) {
        //         auto snap_result = thanos.canSnap(it->second.get(), it2->second.get());
        //         if (snap_result.first) {
        //             num_snaps++;
        //             break;
        //         }
        //     }
        // }
        // std::cout << "can snap: " << num_snaps << " worlds\n";
    }

    void EPRUnit::factorWorlds(QuMvN* qumvn, double epsilon) {
        int num_snaps = 0;
        ThanosUnit thanos;
        std::cout << "worlds before: " << qumvn->size() << "\n";
        for (auto it=qumvn->begin(); it != qumvn->end(); ++it) {
            QuWorld* world1 = it->second.get();
            // std::cout << "checking world: " << world1->getId() << "\n";
            std::cout << thanos.getQuDna(it->second.get()) << "\n";
            /*
            auto it2 = qumvn->begin();
            while (it2 != qumvn->end()) {
                if (it != it2) {
                QuWorld* world2 = it2->second.get();
                auto diff = world1->getWorldAmplitude() - world2->getWorldAmplitude();
                auto canSnap = thanos.canSnap(world1, world2);
                // std::cout << world1->getId() << " vs " << world2->getId() << "\n";
                if (canSnap.first) {
                    std::cout << "snapping\n";
                    num_snaps++;    
                    world1->fuse(world2, canSnap.second);
                    QuAmp64 newAmp = QuAmp64(std::sqrt(2), 0) * world1->getWorldAmplitude();
                    world1->setWorldAmplitude(newAmp);
                    qumvn->fuseWorld(world2);
                    break;
                }
                }
                it2++;
            }
            */
        }
        std::cout << "-----------------------------\n\n";
        std::cout << "worlds after: " << qumvn->size() << "\n"; 
    }
}