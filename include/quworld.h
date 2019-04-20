#ifndef __QUWORLD_H
#define __QUWORLD_H

#include <mkl_vsl.h>
#include "common.h"
/**
 * QuWorlds represents a single source of probabilistic quantum states. You can view this a a Ergodic Discrete
 * Information Source or a World in Many Worlds Quantum Mechanics. It is able to represent any linear combination
 * of NON-ENTANGLED quantum states. Entangled quantum states need to be treated with separate QuWorlds in a
 * Quantum Multiverse Network: QuMvN
 *
 * The thread pattern we use is one thread per worlds, therefore, this class is meant to be accessed by one thread.
 * Having multiple threads update / query this class will not always be thread safe and the caller will be
 * repsonsible for synchronization
 */

class QuWorld {
    private:
        const short int num_qubits;
        const unsigned int id;
        QuAmp64 amplitude;
        QuAmp* qudot_net;
        bool enablingQubit;
        VSLStreamStatePtr stream;

        //void swapDots(int qubitA, QuAmp qBZeroAmp, QuAmp qBOneAmp);
        int getRow(int q) const;

    public:
        QuWorld(short int num_qubits, unsigned int id, QuAmp64 amp, bool bval = true);
        ~QuWorld();

        bool getEnablingQubit() const;
        void setEnablingQubit(bool bval);
        int getId() const;
        void setDotAmplitude(int q, Qubit qval, const QuAmp& amp);
        QuAmp64 getWorldAmplitude() const;
        void setWorldAmplitude(QuAmp64& amp);
        QuAmp getZeroAmplitude(int q) const;
        QuAmp getOneAmplitude(int q) const;
        float getQubitProbability(int q, Qubit qval) const;
        int getNumQubits() const;
        bool isSplitWorlds(int q) const;
        bool isActive(int q, Qubit qval) const;
        bool areActive(int qubits[], int size, Qubit qval) const;
        void deactivate(int q, Qubit qval);
        void deactivateChildren(int q, Qubit qval);
        // will only activate if it does not have zero amplitudes
        void activate(int q, Qubit qval);
        // void swapQubits(int qubitA, int qubitB, Qubit enablingQubit);
        std::string measure();
        bool areNetsEqual(QuWorld& other);
        std::string getWorldSigniture() const;
};

#endif