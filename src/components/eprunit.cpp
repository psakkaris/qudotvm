#include "qudot/components/eprunit.h"

#include <iostream>
#include <unordered_set>
#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;
using std::unordered_set;

namespace qudot {
    void EPRUnit::mergeWorlds(QuMvN* qumvn, double epsilon) {
        std::cout << "num worlds: " << qumvn->size() << "\n";
        std::cout << "epsilon: " << epsilon << "\n";
        unordered_map<string, unordered_set<size_t>> world_sigs;  

        for (auto it=qumvn->begin(); it != qumvn->end(); ++it) {
            QuWorld* quworld = it->second.get();
            string sig = quworld->getWorldSigniture();
            world_sigs[sig].insert(quworld->getId());
        }

        std::cout << world_sigs.size() << " unique world signitures\n";
        for (auto it=world_sigs.begin(); it != world_sigs.end(); ++it) {
            if (it->second.size() > 1) {
                qumvn->mergeWorlds(it->second, epsilon);
            }
        } 
    }
}