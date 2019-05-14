#include "qudot/qumvn.h"

#include <iostream>
#include <ctime>
#include "qudot/common.h"

namespace qudot {

QuMvN::QuMvN(const size_t num_qubits, const size_t multiverse_size) : 
        _num_qubits(num_qubits), _next_world(1), _world_scale_factor(1.0), 
        _world_additive_factor(0.0), _qu_worlds(multiverse_size) 
{
    _qu_worlds[0] = std::make_shared<QuWorld>(_num_qubits, 0, ONE_AMP64);
    vslNewStream(&stream, VSL_BRNG_MT19937, std::clock());
}

size_t QuMvN::getNumQubits() const {
    return _num_qubits;
}

void QuMvN::setNumQubits(const size_t num_qubits) {
    _num_qubits = num_qubits;
}

size_t QuMvN::size() const {
    return _qu_worlds.size();
}

QuWorld* QuMvN::getQuWorld(const size_t index) {
    return _qu_worlds[index].get();
}

std::string QuMvN::measure() {
    QuWorld* quworld = measureWorld();
    return quworld->measure();
}


//**************** PRIVATE METHODS ***********************
double QuMvN::getWorldProbability(const std::shared_ptr<QuWorld> quworld) const {
    QuAmp64 amp = quworld->getWorldAmplitude();
    return ((_world_scale_factor * std::norm(amp) ) + _world_additive_factor);
}

QuWorld* QuMvN::measureWorld() {
    auto ftree = getWorldFenwickTree();

    double r[1];
    const double a=0.0;
    const double b=1.0;

    vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream, 1, r, a, b);
    auto world = ftree->findWorld(r[0]);
    return _qu_worlds[world.first].get();
}

std::shared_ptr<FenwickTree<double>> QuMvN::getWorldFenwickTree() {
    {
        tbb::mutex::scoped_lock lock(_ftree_mutex);
        if (_world_tree) return _world_tree;

        _world_tree = std::make_shared<FenwickTree<double>>(_qu_worlds.size(), TOLERANCE64);
        size_t index = 0;
        for (auto it=_qu_worlds.begin(); it != _qu_worlds.end(); ++it) {
            double probability = getWorldProbability(*it);
            if (probability > 0) {
                _world_tree->update(index, std::make_pair((*it)->getId(), probability));
            }
        }
    }
    return _world_tree;     
}
}