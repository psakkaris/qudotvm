#include "qudot/qumvn.h"

#include <iostream>
#include <ctime>
#include <tbb/parallel_for.h>
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

void QuMvN::swap(const int q1, const int q2, const bool check_enabling_qubit){
    tbb::parallel_for(size_t(0), size_t(_qu_worlds.size()), [&](size_t i) {
        _qu_worlds[i]->swapQubits(q1, q2, check_enabling_qubit);
    });
}

void QuMvN::swap() {
    const int midPoint = _num_qubits / 2;

    for (int i=0; i < midPoint; i++) {
        swap(i+1, _num_qubits-i);
    }    
}

void QuMvN::splitWorlds(const std::vector<int>& ctrls) {
    if (ctrls.empty()) return;
    std::vector<QuWorld*> new_worlds;
    for (auto it=_qu_worlds.begin(); it != _qu_worlds.end(); ++it) {
        for (int ctrl : ctrls) {
            if ( (*it).second->isSplitWorlds(ctrl) ) {
                new_worlds.push_back(createWorld((*it).second.get(), ctrl));
            }
        }
    }

    for (auto it = new_worlds.begin(); it != new_worlds.end(); ++it) {
        std::shared_ptr<QuWorld> s_ptr((*it));
        _qu_worlds[(*it)->getId()] = s_ptr;
    }
}


std::ostream& operator<<(std::ostream& out, const QuMvN& qumvn) {
    out << qumvn._qu_worlds.size() << "," << qumvn._num_qubits << "\n";
    for (auto it=qumvn._qu_worlds.begin(); it != qumvn._qu_worlds.end(); ++it) {
        out << *((*it).second.get());
        if (std::distance(it, qumvn._qu_worlds.end()) != 1) {
            out << "\n";
        }
    }
    return out;
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
            double probability = getWorldProbability((*it).second);
            if (probability > 0) {
                _world_tree->update(index, std::make_pair((*it).second->getId(), probability));
                ++index;
            }
        }
    }
    return _world_tree;     
}

QuWorld* QuMvN::createWorld(QuWorld* old_world, const size_t control_qu) {
    // calculate probability of newWorld and update probability of oldWorld
    QuAmp64 owa = old_world->getWorldAmplitude();
    QuAmp one_amp = old_world->getOneAmplitude(control_qu);
    QuAmp zero_amp = old_world->getZeroAmplitude(control_qu);

    QuAmp64 new_world_amp = owa * QuAmp64(one_amp.real(), one_amp.imag());
    QuAmp64 old_world_amp = owa * QuAmp64(zero_amp.real(), zero_amp.imag());
    old_world->setWorldAmplitude(old_world_amp);

    size_t new_world_id = getNextWorldId();
    QuWorld* new_world = new QuWorld(*old_world);
    new_world->setId(new_world_id);
    new_world->setWorldAmplitude(new_world_amp);
    // Probability of being Zero in oldWorld = 1 and Probability of being One in oldWorld = 0
    old_world->activate(control_qu, ZERO);
    old_world->deactivate(control_qu, ONE);

    // Probability of being Zero in newWorld = 0 and Probability of being One in  newWorld = 1
    new_world->activate(control_qu, ONE);
    new_world->deactivate(control_qu, ZERO);

    if (control_qu < _num_qubits) {
        // children of 1 should be inactive in old world
        old_world->deactivateChildren(control_qu, ONE);
        // children of 0 should be inactive in new world
        new_world->deactivateChildren(control_qu, ZERO);        
    }
    return new_world;
}

size_t QuMvN::getNextWorldId() {
    tbb::mutex::scoped_lock lock(_next_world_mutex);
    return _next_world++;
}

}