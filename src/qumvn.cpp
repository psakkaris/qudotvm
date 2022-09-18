#include "qudot/qumvn.h"

#include <boost/log/trivial.hpp>

#include <cmath>
#include <iostream>
#include <ctime>
#include <unordered_set>
#include <numeric>
#include <functional>

#include "tbb/parallel_for.h"
#include "tbb/concurrent_vector.h"
#include "tbb/concurrent_unordered_set.h"
#include "tbb/parallel_reduce.h"

#include "qudot/common.h"

namespace qudot {

QuMvN::QuMvN(const size_t num_qubits, const size_t multiverse_size) : 
        _num_qubits(num_qubits), _next_world(1), _world_scale_factor(1.0), 
        _world_additive_factor(0.0)
{
    _quworlds.rehash(multiverse_size);
    WorldMap::accessor a;
    _quworlds.insert(a, 0);
    a->second = std::make_shared<QuWorld>(_num_qubits, 0, ONE_AMP64);
    a.release();
    vslNewStream(&stream, VSL_BRNG_MT19937, std::clock());
}

size_t QuMvN::getNumQubits() const {
    return _num_qubits;
}

size_t QuMvN::getNextWorld() const {
    return _next_world;
}

void QuMvN::setNumQubits(const size_t num_qubits) {
    _num_qubits = num_qubits;
}

size_t QuMvN::size() const {
    return _quworlds.size();
}

QuWorld* QuMvN::getQuWorld(const size_t index) {
    WorldMap::const_accessor ca;
    _quworlds.find(ca, index);
    if (ca.empty()) {
        return nullptr;
    } else {
        return ca->second.get();
    }
}

std::string QuMvN::measure() {
    double r[_num_qubits];
    const double a=0.0;
    const double b=1.0;

    vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream, _num_qubits, r, a, b);
    QuWorld* quworld = measureWorld();
    return quworld->measure(r);
}

std::string QuMvN::measure(const double rands[]) {
    QuWorld* quworld = measureWorld();
    return quworld->measure(rands);
}

Qubit QuMvN::measureQubit(const size_t q) {
    BOOST_LOG_TRIVIAL(info) << "measure qubit: reduce";
    Qubit activeq;
    Qubit deactiveq;
    float zero_prob = tbb::parallel_reduce(range(), 0.f, [&] (const WorldMap::const_range_type &r, float value) -> float {
        for (auto it=r.begin(); it != r.end(); ++it) {
            value += getWorldProbability((*it).second) * (*it).second->getQubitProbability(q, ZERO);
        }        
        return value;
    },
    std::plus<float>() 
    );

    double rand = getRand();
    if ((rand + TOLERANCE <= zero_prob) || (rand - TOLERANCE) <= zero_prob) {
        activeq = ZERO;
        deactiveq = ONE;
    } else {
        activeq = ONE;
        deactiveq = ZERO;
    }    
    BOOST_LOG_TRIVIAL(info) << "measure qubit: kill worlds";
    tbb::concurrent_unordered_set<size_t> deadworlds;
    tbb::parallel_for(range(), [&] (const WorldMap::const_range_type &r) {
        for (auto it = r.begin(); it != r.end(); ++it) {
            (*it).second->activate(q, activeq);
            if ((*it).second->getQubitProbability(q, deactiveq) >= 1.0 - TOLERANCE) {
                deadworlds.insert((*it).second->getId());
            } else {
                (*it).second->deactivate(q, deactiveq);
            }        
        }        
    });
    BOOST_LOG_TRIVIAL(info) << "remove dead worlds";
    tbb::parallel_for(deadworlds.range(), [&] (const tbb::concurrent_unordered_set<size_t>::range_type &r) {
        for (auto it=r.begin(); it != r.end(); ++it) {
            QuWorld* quworld = getQuWorld(*it);
            if (quworld) {
                removeWorld(quworld);
            }
        }
    });

    // for (auto it = _quworlds.begin(); it != _quworlds.end(); ++it) {
    //     (*it).second->activate(q, activeq);
    //     if ((*it).second->getQubitProbability(q, deactiveq) >= 1.0 - TOLERANCE) {
    //         deadworlds.insert((*it).second->getId());
    //     } else {
    //         (*it).second->deactivate(q, deactiveq);
    //     }        
    // }
    // for (auto it=deadworlds.begin(); it != deadworlds.end(); ++it) {
    //     QuWorld* quworld = getQuWorld(*it);
    //     if (quworld) {
    //         removeWorld(quworld);
    //     }
    // }
    BOOST_LOG_TRIVIAL(info) << "done";

    return activeq;
}

void QuMvN::swap(const int q1, const int q2, const bool check_enabling_qubit){
    tbb::parallel_for(range(), [&] (const WorldMap::const_range_type &r) {
        for (auto it=r.begin(); it != r.end(); ++it) {
            (*it).second->swapQubits(q1, q2, check_enabling_qubit);    
        }
    });
}

void QuMvN::swap() {
    const int midPoint = _num_qubits / 2;

    for (int i=0; i < midPoint; i++) {
        swap(i+1, _num_qubits-i);
    }    
}

void QuMvN::splitAllWorlds() {
    std::vector<int> ctrls;
    for (size_t i=1; i <= _num_qubits; i++) {
        ctrls.push_back(i);
    }
    splitWorlds(ctrls);    
}

void QuMvN::splitWorlds(const std::vector<int>& ctrls) {
    if (ctrls.empty()) return; 

    tbb::concurrent_vector<QuWorld*> new_worlds;
    tbb::parallel_for(constRange(), [&](const WorldMap::const_range_type &r) {
        for (auto it = r.begin(); it != r.end(); it++) {
            for (int ctrl : ctrls) {
                if ( (*it).second->isSplitWorlds(ctrl) ) {
                    new_worlds.push_back(createWorld((*it).second.get(), ctrl));
                }
            }
        }
    });

    tbb::parallel_for(size_t(0), size_t(new_worlds.size()), [&] (size_t i) {
        std::shared_ptr<QuWorld> s_ptr(new_worlds[i]);
        WorldMap::accessor a;
        _quworlds.insert(a, new_worlds[i]->getId());
        a->second = s_ptr;
        //_quworlds[new_worlds[i]->getId()] = s_ptr;
    });
}

WorldMap::iterator QuMvN::begin() {
    return _quworlds.begin();
}

WorldMap::iterator QuMvN::end() {
    return _quworlds.end();
}

WorldMap::range_type QuMvN::range() {
    return _quworlds.range(5000);
}

WorldMap::const_range_type QuMvN::constRange() const {
    return _quworlds.range(10000);
}


std::ostream& operator<<(std::ostream& out, const QuMvN& qumvn) {
    out << qumvn._quworlds.size() << "," << qumvn._num_qubits << "\n";
    for (auto it=qumvn._quworlds.begin(); it != qumvn._quworlds.end(); ++it) {
        out << *((*it).second.get());
        if (std::distance(it, qumvn._quworlds.end()) != 1) {
            out << "\n";
        }
    }
    return out;
}

double QuMvN::getWorldProbability(const QuAmp64& amp) const {
    return ((_world_scale_factor * std::norm(amp) ) + _world_additive_factor);
}

void QuMvN::removeWorld(const size_t world_id) {
    WorldMap::const_accessor a;
    _quworlds.find(a, world_id);
    removeWorld(a->second.get());
}

void QuMvN::mergeWorlds(const tbb::concurrent_unordered_set<size_t>& worlds, double epsilon) {
    QuAmp64 new_amp = ZERO_AMP64;
    double new_prob = 0.0;
    for (auto it=worlds.begin(); it != worlds.end(); ++it) {
        QuWorld* quworld = getQuWorld(*it);
        QuAmp64 amp = quworld->getWorldAmplitude();
        new_amp += amp;
        new_prob += getWorldProbability(amp);
    }
    // note below assumes that qudotnets are identical
    // is zero
    if (!isNotZero(new_amp, epsilon)) {
        // worlds cancel
        for (auto it=worlds.begin(); it != worlds.end(); ++it) {
            //_quworlds.erase(*it);
            getQuWorld(*it)->setWorldAmplitude(const_cast<QuAmp64 &>(ZERO_AMP64));
        }
        //tbb::mutex::scoped_lock lock(_remove_world_mutex);
        //_world_additive_factor = new_prob / _quworlds.size();
    } else {
        // worlds add up
        auto it = worlds.begin();
        QuWorld* model_world = getQuWorld(*it);
        //QuAmp64 myamp(std::sqrt(new_prob), 0.0);
        model_world->setWorldAmplitude(new_amp);
        ++it;
        for (; it != worlds.end(); ++it) {
            _quworlds.erase(*it);
        }
    }    
}

//**************** PRIVATE METHODS ***********************
double QuMvN::getWorldProbability(const std::shared_ptr<QuWorld> quworld) const {
    QuAmp64 amp = quworld->getWorldAmplitude();
    return getWorldProbability(amp);
}

double QuMvN::getRand() {
    double r[1];
    vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream, 1, r, ka, kb);
    return r[0];
}

QuWorld* QuMvN::measureWorld() {
    auto ftree = getWorldFenwickTree();
    auto rand = getRand();
    auto world = ftree->findWorld(rand);
    while (_quworlds.count(world.first) <= 0) {
        BOOST_LOG_TRIVIAL(warning) << "NULL WORLD";
        rand = getRand();
        world = ftree->findWorld(rand);
    }
    WorldMap::const_accessor ca;
    _quworlds.find(ca, world.first);
    return ca->second.get();
}

std::shared_ptr<FenwickTree<double>> QuMvN::getWorldFenwickTree() {
    {
        tbb::mutex::scoped_lock lock(_ftree_mutex);
        if (_world_tree) return _world_tree;

        _world_tree = std::make_shared<FenwickTree<double>>(_quworlds.size(), TOLERANCE64);
        size_t index = 0;
        double tot_prob = 0.0;
        for (auto it=_quworlds.begin(); it != _quworlds.end(); ++it) {
            double probability = getWorldProbability((*it).second);
            if (probability > 0) {
                tot_prob += probability;
                _world_tree->update(index, std::make_pair((*it).second->getId(), probability));
                ++index;
            }
        }
        BOOST_LOG_TRIVIAL(info) << "total probability: " << tot_prob;
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

void QuMvN::removeWorld(QuWorld* quworld) {
    double scale_factor = 1.0 / (1 - getWorldProbability(quworld->getWorldAmplitude()));
    tbb::mutex::scoped_lock lock(_remove_world_mutex);
    _world_scale_factor = _world_scale_factor * scale_factor;
    lock.release();
    _quworlds.erase(quworld->getId());  
}

}
