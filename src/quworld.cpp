#include "common.h"
#include "quworld.h"

QuWorld::QuWorld(short int _num_qubits, unsigned int id, QuAmp amp, bool bval) : 
        num_qubits(_num_qubits), id(id), amplitude(amp), enablingQubit(bval)
{
    qudot_net = new QuAmp[num_qubits*4];   
    for (int i=0; i < num_qubits; i++) {
        const int row = i*qu_stride;
        qudot_net[row + 0] = 1;
        qudot_net[row + 1] = 0;
        qudot_net[row + 2] = 0;
        qudot_net[row + 3] = 0;
    } 
}

QuWorld::~QuWorld() {
    delete[] qudot_net;
}

int QuWorld::getNumQubits() const {
    return num_qubits;
}

int QuWorld::getId() const {
    return id;
}

QuAmp QuWorld::getWorldAmplitude() const {
    return amplitude;
}

void QuWorld::setWorldAmplitude(QuAmp& amp) {
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