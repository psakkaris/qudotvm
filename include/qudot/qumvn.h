#ifndef __QUDOT_QUMVN_H
#define __QUDOT_QUMVN_H

#include <cstddef>
#include <iterator>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "mkl_vsl.h"
#include "tbb/mutex.h"

#include "qudot/fenwicktree.hpp"
#include "qudot/measurable.h"
#include "qudot/quworld.h"

namespace qudot {

class QuMvN : public Measurable {
    private:
        size_t _num_qubits;
        size_t _next_world;
        double _world_scale_factor;
        double _world_additive_factor;
        tbb::mutex _ftree_mutex;
        tbb::mutex _next_world_mutex;
        VSLStreamStatePtr stream;

        std::unordered_map<size_t, std::shared_ptr<QuWorld>> _qu_worlds;
        std::shared_ptr<FenwickTree<double>> _world_tree;

        double getWorldProbability(const std::shared_ptr<QuWorld>) const;
        QuWorld* measureWorld();
        QuWorld* createWorld(QuWorld* old_world, const size_t control_qu);
        std::shared_ptr<FenwickTree<double>> getWorldFenwickTree();
        size_t getNextWorldId();

    public:
        QuMvN(const size_t, const size_t);   
        size_t getNumQubits() const;
        void setNumQubits(const size_t); 
        size_t size() const;
        QuWorld* getQuWorld(const size_t);
        std::string measure() override;
        // swapping functions
        void swap(const int, const int, const bool = false);
        void swap();
        void splitWorlds(const std::vector<int>&);
};
    
}

#endif