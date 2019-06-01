#ifndef __QUDOT_QUWORLD_H
#define __QUDOT_QUWORLD_H

#include <cstddef>
#include <iostream>
#include <vector>
#include <utility>

#include "mkl_vsl.h"
#include "common.h"
#include "measurable.h"

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

namespace qudot { 
    // need to forward declare QuGate so we can mark as friend
    class X;
    class Y;
    class H;
    class R;

    class QuWorld {
        private:
            int num_qubits;
            size_t id;
            QuAmp64 amplitude;
            QuAmp* qudot_net;
            bool enabling_q[2];

            void swapDots(const int qubit_a, const QuAmp& qb_zero_amp, const QuAmp& qb_one_amp);
            int getRow(int q) const;
            void copyRegisterQubits(QuAmp dest[], const std::pair<int, int>& qureg, int& index);
            void linkRegisters(const int parent, const int qubit);

        public:
            QuWorld(int num_qubits, size_t id, QuAmp64 amp);
            QuWorld(const QuWorld&);
            QuWorld(QuWorld &&) noexcept;
            ~QuWorld();

            QuWorld& operator=(const QuWorld&);
            QuWorld& operator=(QuWorld &&) noexcept;

            std::string measure(const double rands[]);
            bool getEnablingQubit(const Qubit) const;
            void setEnablingQubit(const int q);
            size_t getId() const;
            void setId(const size_t);
            void setDotAmplitude(int q, Qubit qval, const QuAmp& amp);
            QuAmp64 getWorldAmplitude() const;
            void setWorldAmplitude(QuAmp64& amp);
            QuAmp getZeroAmplitude(int q) const;
            QuAmp getOneAmplitude(int q) const;
            float getQubitProbability(int q, Qubit qval) const;
            int getNumQubits() const;
            bool isSplitWorlds(int q) const;
            bool isActive(const int q, const Qubit qval) const;
            bool areActive(const std::vector<int>&, const Qubit qval) const;
            void deactivate(int q, Qubit qval);
            void deactivateChildren(int q, Qubit qval);
            // will only activate if it does not have zero amplitudes
            void activate(int q, Qubit qval);
            void swapQubits(const int qubit_a, const int qubit_b, bool check_enabling_qubit=false);
            bool areNetsEqual(const QuWorld& other);
            std::string getWorldSigniture() const;
            // expand / contract number of qubits
            void expandQubits(const int nq);
            void contractQubits(const int nq, const std::pair<int, int>& qubits_above, const std::pair<int, int>& qubits_result, const std::pair<int, int>& qubits_below);

        friend std::ostream& operator<<(std::ostream&, const QuWorld&);    

        friend X;
        friend Y;
        friend H;
        friend R;
    };
}

#endif