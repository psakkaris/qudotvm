#include "qudot/qudotconfig.h"

namespace qudot {
    QuDotConfig::QuDotConfig() : num_qubits(0), ensemble_size(0), stack_size(DEFAULT_STACK_SIZE),
                                print_results(false), print_worlds(false), multiverse_size(DEFAULT_MULITVERSE_SIZE) {}

    unsigned int QuDotConfig::getNumQubits() const {
        return num_qubits;
    }

    void QuDotConfig::setNumQubits(const unsigned int nq) {
        num_qubits = nq;
    }

    unsigned int QuDotConfig::getEnsembleSize() const {
        return ensemble_size;
    }

    void QuDotConfig::setEnsembleSize(const unsigned int es) {
        ensemble_size = es;
    }

    unsigned int QuDotConfig::getStackSize() const {
        return stack_size;
    }

    void QuDotConfig::setStackSize(const unsigned int ss) {
        stack_size = ss;
    }

    bool QuDotConfig::getPrintResults() const {
        return print_results;
    }

    void QuDotConfig::setPrintResults(const bool pr) {
        print_results = pr;
    }

    bool QuDotConfig::getPrintWorlds() const {
        return print_worlds;
    }

    void QuDotConfig::setPrintWorlds(const bool qupw) {
        print_worlds = qupw;
    }

    unsigned int QuDotConfig::getMultiverseSize() const {
        return multiverse_size;
    }

    void QuDotConfig::setMultiverseSize(const unsigned int ms) {
        multiverse_size = ms;
    }
} // namespace qudot