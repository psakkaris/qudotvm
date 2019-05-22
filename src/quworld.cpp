#include "qudot/quworld.h"

#include <ctime>
#include <iostream>
#include <string>

#include <mkl_vsl.h>

#include "qudot/common.h"

namespace qudot {
    QuWorld::QuWorld(int _num_qubits, size_t _id, QuAmp64 amp) : 
            num_qubits(_num_qubits), id(_id), amplitude(amp), enabling_q{true, true}
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

    QuWorld::QuWorld(const QuWorld& other) : num_qubits(other.num_qubits), id(other.id), amplitude(other.amplitude), enabling_q{other.enabling_q[0], other.enabling_q[1]} {
        std::cout << "COPY CONSTRUCTOR\n";
        size_t sz = num_qubits*4;
        qudot_net = new QuAmp[sz];
        for (size_t i=0; i < sz; i++) {
            qudot_net[i] = other.qudot_net[i];
        }
        vslNewStream(&stream, VSL_BRNG_MT19937, std::clock());
    }

    QuWorld::QuWorld(QuWorld&& other) noexcept : num_qubits(0), id(0), amplitude(0), enabling_q{true, true} {
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

    void QuWorld::setId(const size_t _id) {
        id = _id;
    }

    QuAmp64 QuWorld::getWorldAmplitude() const {
        return amplitude;
    }

    void QuWorld::setWorldAmplitude(QuAmp64& amp) {
        amplitude = amp;
    }

    bool QuWorld::getEnablingQubit(const Qubit qval) const {
        if (qval == ZERO) return enabling_q[0];

        return enabling_q[1];
    }

    void QuWorld::setEnablingQubit(const int q) {
        enabling_q[0] = isActive(q, ZERO);
        enabling_q[1] = isActive(q, ONE);
    }

    bool QuWorld::isActive(int q, Qubit qval) const {
        int row = getRow(q);
        if (qval == ZERO) {
            return isNotZero(qudot_net[row]) || isNotZero(qudot_net[row+2]);
        } else {
            return isNotZero(qudot_net[row+1]) || isNotZero(qudot_net[row+3]);
        }
    }

    bool QuWorld::areActive(const std::vector<int>& qubits, const Qubit qval) const {
        for (auto it=qubits.begin(); it != qubits.end(); ++it) {
            if (!isActive((*it), qval)) {
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
        
        if (isNotZero(qudot_net[row+qval])) qudot_net[row+qval] = ONE_AMP;
        if (isNotZero(qudot_net[row+qval+2])) qudot_net[row+qval+2] = ONE_AMP;
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
        if (!check_enabling_qubit) {
            QuAmp qa_zero_amp = getZeroAmplitude(qubit_a);
            QuAmp qa_one_amp = getOneAmplitude(qubit_a);
            QuAmp qb_zero_amp = getZeroAmplitude(qubit_b);
            QuAmp qb_one_amp = getOneAmplitude(qubit_b);

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

    void QuWorld::expandQubits(const int nq) {
        int total_qubits = num_qubits + nq;
        QuAmp* expanded_net = new QuAmp[total_qubits*qu_stride];

        for (int q=0; q < nq; q++) {
            int row = q*qu_stride;
            expanded_net[row] = ONE_AMP;
            for (int i=1; i < qu_stride; i++) {
                expanded_net[row + i] = ZERO_AMP;
            }
        }

        int i = 0;
        for (int q=nq*qu_stride; q < total_qubits*qu_stride; q++,i++) {
            expanded_net[q] = qudot_net[i];
        }

        delete[] qudot_net;
        qudot_net = expanded_net;
        num_qubits = total_qubits;
    }

    void QuWorld::contractQubits(const int nq, const std::pair<int, int>& qubits_above, const std::pair<int, int>& qubits_result, const std::pair<int, int>& qubits_below){
        QuAmp* contracted_qudotnet = new QuAmp[nq*qu_stride];
        int index = 0;
        copyRegisterQubits(contracted_qudotnet, qubits_above, index);
        copyRegisterQubits(contracted_qudotnet, qubits_result, index);
        copyRegisterQubits(contracted_qudotnet, qubits_below, index);
        delete[] qudot_net;
        qudot_net = contracted_qudotnet;
        // handle root
        if (isActive(1, ZERO)) {
            qudot_net[0] = getZeroAmplitude(1);
            qudot_net[2] = ZERO_AMP;
        }
        if (isActive(1, ONE)) {
            qudot_net[1] = getOneAmplitude(1);
            qudot_net[3] = ZERO_AMP;
        }

        int num_qubits_above = qubits_above.second - qubits_above.first + 1;
        int num_qubits_below = qubits_below.second - qubits_below.first + 1;
        int num_qubits_result = qubits_result.second - qubits_result.first + 1;

        if (num_qubits_above > 0) {
            linkRegisters(num_qubits_above, num_qubits_above+1);
        }

        if (num_qubits_below > 0) {
            linkRegisters(num_qubits_above + num_qubits_result, num_qubits_above + num_qubits_result + 1);
        }

        num_qubits = nq;
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

    void QuWorld::copyRegisterQubits(QuAmp dest[], const std::pair<int, int>& qureg, int& index) {
        for (int q=qureg.first; q <= qureg.second; q++) {
            int row = getRow(q);
            for (int i=0; i < qu_stride; i++, index++) {
                dest[index] = qudot_net[row+i];
            }
        }
    }

    void QuWorld::linkRegisters(const int parent, const int qubit) {
        auto zeroAmp = getZeroAmplitude(qubit);
        auto oneAmp = getOneAmplitude(qubit);

        // wipe out any existing values of the qubit register then set parent amplitudes to parentQubit values
        deactivate(qubit, ZERO);
        deactivate(qubit, ONE);

        int row = getRow(qubit);
        if (isActive(parent, ZERO)) {
            qudot_net[row + 0] = zeroAmp;
            qudot_net[row + 1] = oneAmp;
        }
        if (isActive(parent, ONE)) {
            qudot_net[row + 2] = zeroAmp;
            qudot_net[row + 3] = oneAmp;
        }
    }

    std::ostream& operator<<(std::ostream& out, const QuWorld& quworld) {
        int end = (quworld.getNumQubits() * qu_stride) - 1;
        out << quworld.amplitude << "\n";
        for (int i=0; i < quworld.getNumQubits(); i++) {
            for (int k=0; k < qu_stride; k++) {
                int index = (i * qu_stride) + k;
                out << quworld.qudot_net[index];
                if (index != end) {
                    out << ",";
                }
            }
        }
        return out;
    }
} // namespace qudot