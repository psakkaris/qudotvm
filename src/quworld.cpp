#include "qudot/quworld.h"

#include <ctime>
#include <iostream>
#include <string>

#include <mkl_vsl.h>

#include "qudot/common.h"

namespace qudot {
    QuWorld::QuWorld(short int _num_qubits, size_t _id, QuAmp64 amp, bool bval) : 
            num_qubits(_num_qubits), id(_id), amplitude(amp), enablingQubit(bval)
    {
        qudot_net = new QuAmp[num_qubits*4];   
        for (int i=0; i < num_qubits; i++) {
            const int row = i*qu_stride;
            qudot_net[row + 0] = 1;
            qudot_net[row + 1] = 0;
            qudot_net[row + 2] = 0;
            qudot_net[row + 3] = 0;
        } 
        vslNewStream(&stream, VSL_BRNG_MT19937, std::clock());
    }

    QuWorld::QuWorld(const QuWorld& other) : num_qubits(other.num_qubits), id(other.id), amplitude(other.amplitude), enablingQubit(other.enablingQubit) {
        std::cout << "COPY CONSTRUCTOR\n";
        size_t sz = num_qubits*4;
        qudot_net = new QuAmp[sz];
        for (size_t i=0; i < sz; i++) {
            qudot_net[i] = other.qudot_net[i];
        }
        vslNewStream(&stream, VSL_BRNG_MT19937, std::clock());
    }

    QuWorld::QuWorld(QuWorld&& other) noexcept : num_qubits(0), id(0), amplitude(0), enablingQubit(0) {
        std::cout << "MOVE CONSTRUCTOR\n";
    }

    QuWorld& QuWorld::operator=(const QuWorld& other) {
        std::cout << "ASSIGNMENT OPERATOR\n";
        return *this;
    }

    QuWorld& QuWorld::operator=(QuWorld&& other) noexcept {
        std::cout << "MOVE ASSIGNMENT OPERATOR\n";
        return *this;
    }

    QuWorld::~QuWorld() {
        delete[] qudot_net;
        qudot_net = nullptr;
        
        vslDeleteStream(&stream);
    }

    int QuWorld::getNumQubits() const {
        return num_qubits;
    }

    size_t QuWorld::getId() const {
        return id;
    }

    QuAmp64 QuWorld::getWorldAmplitude() const {
        return amplitude;
    }

    void QuWorld::setWorldAmplitude(QuAmp64& amp) {
        amplitude = amp;
    }

    bool QuWorld::getEnablingQubit() const {
        return enablingQubit;
    }

    void QuWorld::setEnablingQubit(bool bval) {
        enablingQubit = bval;
    }

    bool QuWorld::isActive(int q, Qubit qval) const {
        int row = getRow(q);
        if (qval == ZERO) {
            return isNotZero(qudot_net[row]) || isNotZero(qudot_net[row+2]);
        } else {
            return isNotZero(qudot_net[row+1]) || isNotZero(qudot_net[row+3]);
        }
    }

    bool QuWorld::areActive(int qubits[], int size, Qubit qval) const {
        for (int i=0; i < size; i++) {
            if (!isActive(qubits[i], qval)) {
                return false;
            }
        }
        return true;
    }

    int QuWorld::getRow(int q) const {
        return (q-1) * qu_stride;
    }

    QuAmp QuWorld::getZeroAmplitude(int q) const {
        int row = getRow(q);
        QuAmp amp = qudot_net[row];
        if (isNotZero(amp)) {
            return amp;
        }

        return qudot_net[row+2];
    }

    QuAmp QuWorld::getOneAmplitude(int q) const {
        int row = getRow(q);
        QuAmp amp = qudot_net[row+1];
        if (isNotZero(amp)) {
            return amp;
        }

        return qudot_net[row+3];
    }

    void QuWorld::setDotAmplitude(int q, Qubit qval, const QuAmp& amp) {
        qudot_net[getRow(q) + qval] = amp;
        qudot_net[getRow(q) + qval + 2] = amp;
    }

    float QuWorld::getQubitProbability(int q, Qubit qval) const {
        QuAmp amp;
        if (qval == ZERO) {
            amp = getZeroAmplitude(q);
        } else {
            amp = getOneAmplitude(q);
        }
        return std::norm(amp);
    }

    bool QuWorld::isSplitWorlds(int q) const {
        return isActive(q, ZERO) && isActive(q, ONE);
    }

    void QuWorld::deactivate(int q, Qubit qval) {
        int row = getRow(q);
        qudot_net[row+qval] = ZERO_AMP;
        qudot_net[row+qval+2] = ZERO_AMP;
    }

    void QuWorld::activate(int q, Qubit qval) {
        int row = getRow(q);
        if (isActive(q, qval)) {
            qudot_net[row+qval] = ONE_AMP;
            qudot_net[row+qval+2] = ONE_AMP;
        }
    }

    void QuWorld::deactivateChildren(int q, Qubit qval) {
        int row = getRow(q+1);
        if (qval == ZERO) {
            qudot_net[row] = ZERO_AMP;
            qudot_net[row+1] = ZERO_AMP;
        } else {
            qudot_net[row+2] = ZERO_AMP;
            qudot_net[row+3] = ZERO_AMP;
        }
    }

    bool QuWorld::areNetsEqual(const QuWorld& other) {
        if (num_qubits != other.getNumQubits()) {
            return false;
        }
        QuAmp delta = ZERO_AMP;
        for (int i=0; i < num_qubits; i++) {
            const int stride = i*qu_stride;
            for (int j=0; j < qu_stride; j++) {
                delta += qudot_net[stride + j] - other.qudot_net[stride + j];        
            }
        }
        return !isNotZero(delta);
    }

    std::string QuWorld::measure() {
        double r[1];   
        const double a=0.0;
        const double b=1.0;

        std::string result = "";
        int m=0;
        for (int q=0; q < num_qubits; q++) {
                bool zeroActive = isActive(q+1, ZERO);
                bool oneActive = isActive(q+1, ONE);

                // The isActive test above takes into account our tolerence level for machine epsilon
                // also doing a test for 0/1 active first let's us skip random number generation for very many cases
                if (zeroActive && oneActive) {
                    vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream, 1, r, a, b);
                    double randNum = r[0];
                    double zeroProb = getQubitProbability(q+1, ZERO);
                    if ((randNum + TOLERANCE <= zeroProb) || (randNum - TOLERANCE) <= zeroProb) {
                        m = 0;
                    } else {
                        m = 1;
                    }
                } else if (zeroActive) {
                    m = 0;
                } else if (oneActive) {
                    m = 1;
                }
                result += QUBIT_LABEL[m];
        }
        return result;
    }

    void QuWorld::swapQubits(const int qubit_a, const int qubit_b, bool check_enabling_qubit) {
        if (!check_enabling_qubit || enablingQubit) {
            QuAmp qa_zero_amp = QuAmp(getZeroAmplitude(qubit_a));
            QuAmp qa_one_amp = QuAmp(getOneAmplitude(qubit_a));
            QuAmp qb_zero_amp = QuAmp(getZeroAmplitude(qubit_b));
            QuAmp qb_one_amp = QuAmp(getOneAmplitude(qubit_b));

            swapDots(qubit_a, qb_zero_amp, qb_one_amp);
            swapDots(qubit_b, qa_zero_amp, qa_one_amp);
        }
    }

    std::string QuWorld::getWorldSigniture() const {
        std::string sig = "";
        for (int q=1; q <= num_qubits; q++) {
            sig += (isActive(q, ZERO) ? "0a" : "0d");
            sig += (isActive(q, ONE)  ? "1a" : "1d");
        }
        return sig;
    }

    //#################### PRIVATE #####################/

    void QuWorld::swapDots(const int qubit_a, const QuAmp& qb_zero_amp, const QuAmp& qb_one_amp) {
        // The active parents of qa point to the active qb dots
        const int qubit_a_parents = qubit_a-1;
        if (qubit_a == 1) {
            int row = getRow(qubit_a);
            qudot_net[row + 0] = qb_zero_amp;
            qudot_net[row + 1] = qb_one_amp;
        } else {
            int row = getRow(qubit_a);
            if (isActive(qubit_a_parents, ZERO)) {
                qudot_net[row + 0] = qb_zero_amp;
                qudot_net[row + 1] = qb_one_amp;
            }   
            if (isActive(qubit_a_parents, ONE)) {
                qudot_net[row + 2] = qb_zero_amp;
                qudot_net[row + 3] = qb_one_amp;
            } 
        }

        // The active children of qa need to remain the same
        const int qubit_a_children = qubit_a + 1;
        if (qubit_a < num_qubits) {
            int row = getRow(qubit_a_children);
            QuAmp qa_child_zero_amp = getZeroAmplitude(qubit_a_children);
            QuAmp qa_child_one_amp = getOneAmplitude(qubit_a_children);
            bool zeroChildActive = isActive(qubit_a_children, ZERO);
            bool oneChildActive = isActive(qubit_a_children, ONE);
            bool zeroActive = isActive(qubit_a, ZERO);
            bool oneActive = isActive(qubit_a, ONE);

            for (int i=0; i < qu_stride; i++) {
                qudot_net[row+i] = ZERO_AMP;
            }

            if (zeroChildActive) {
                if (zeroActive) {
                    qudot_net[row + 0] = qa_child_zero_amp;
                }
                if (oneActive) {
                    qudot_net[row + 2] = qa_child_zero_amp;
                }
            }

            if (oneChildActive) {
                if (zeroActive) {
                    qudot_net[row + 1] = qa_child_one_amp;
                }
                if (oneActive) {
                    qudot_net[row + 3] = qa_child_one_amp;
                }
            }
        } 
    }
} // namespace qudot