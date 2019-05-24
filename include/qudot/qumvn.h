#ifndef __QUDOT_QUMVN_H
#define __QUDOT_QUMVN_H

#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "mkl_vsl.h"
#include "tbb/mutex.h"

#include "qudot/common.h"
#include "qudot/fenwicktree.hpp"
#include "qudot/measurable.h"
#include "qudot/quworld.h"

namespace qudot {

typedef std::unordered_map<size_t, std::shared_ptr<QuWorld>> WorldMap;
class QuMvN : public Measurable {
    private:
        static double constexpr ka = 0.0;
        static double constexpr kb = 1.0;

        size_t _num_qubits;
        size_t _next_world;
        double _world_scale_factor;
        double _world_additive_factor;
        tbb::mutex _ftree_mutex;
        tbb::mutex _next_world_mutex;
        VSLStreamStatePtr stream;

        WorldMap _qu_worlds;
        std::shared_ptr<FenwickTree<double>> _world_tree;

        double getRand();
        double getWorldProbability(const std::shared_ptr<QuWorld>) const;
        void removeWorld(QuWorld* quworld);
        QuWorld* measureWorld();
        QuWorld* createWorld(QuWorld* old_world, const size_t control_qu);
        std::shared_ptr<FenwickTree<double>> getWorldFenwickTree();
        size_t getNextWorldId();

    public:
        QuMvN(const size_t, const size_t);   
        size_t getNumQubits() const;
        double getWorldProbability(const QuAmp64& amp) const;
        void setNumQubits(const size_t); 
        size_t size() const;
        QuWorld* getQuWorld(const size_t);
        std::string measure() override;
        Qubit measureQubit(const size_t);
        // swapping functions
        void swap(const int, const int, const bool = false);
        void swap();
        void splitWorlds(const std::vector<int>&);
        void splitAllWorlds();
        void removeWorld(const size_t id);
        void mergeWorlds(const std::unordered_set<size_t>& worlds, double epsilon = TOLERANCE64);
        WorldMap::iterator begin();
        WorldMap::iterator end();

    friend std::ostream& operator<<(std::ostream&, const QuMvN&);
};
    
}

#endif