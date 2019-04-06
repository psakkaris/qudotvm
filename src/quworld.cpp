#include "common.h"
#include "quworld.h"

QuWorld::QuWorld(short int numQubits, unsigned int id, QuAmp amp, bool bval) : 
        numQubits(numQubits), id(id), amplitude(amp), enablingQubit(bval)
{
    qudotNet = new QuAmp[numQubits*4];   
    for (int i=0; i < numQubits; i++) {
        const int row = i*qu_stride;
        qudotNet[row + 0] = 1;
        qudotNet[row + 1] = 0;
        qudotNet[row + 2] = 0;
        qudotNet[row + 3] = 0;
    } 
}

QuWorld::~QuWorld() {
    delete[] qudotNet;
}

int QuWorld::getNumQubits() const {
    return numQubits;
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
        return isNotZero(qudotNet[row]) || isNotZero(qudotNet[row+2]);
    } else {
        return isNotZero(qudotNet[row+1]) || isNotZero(qudotNet[row+3]);
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
    QuAmp amp = qudotNet[row];
    if (isNotZero(amp)) {
        return amp;
    }

    return qudotNet[row+2];
}

QuAmp QuWorld::getOneAmplitude(int q) const {
    int row = getRow(q);
    QuAmp amp = qudotNet[row+1];
    if (isNotZero(amp)) {
        return amp;
    }

    return qudotNet[row+3];
}

void QuWorld::setDotAmplitude(int q, Qubit qval, const QuAmp& amp) {
    qudotNet[getRow(q) + qval] = amp;
    qudotNet[getRow(q) + qval + 2] = amp;
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
    qudotNet[row+qval] = ZERO_AMP;
    qudotNet[row+qval+2] = ZERO_AMP;
}

void QuWorld::activate(int q, Qubit qval) {
    int row = getRow(q);
    if (isActive(q, qval)) {
        qudotNet[row+qval] = ONE_AMP;
        qudotNet[row+qval+2] = ONE_AMP;
    }
}

void QuWorld::deactivateChildren(int q, Qubit qval) {
    int row = getRow(q+1);
    if (qval == ZERO) {
        qudotNet[row] = ZERO_AMP;
        qudotNet[row+1] = ZERO_AMP;
    } else {
        qudotNet[row+2] = ZERO_AMP;
        qudotNet[row+3] = ZERO_AMP;
    }
}