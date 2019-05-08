#include "qudot/gates/R.h"

#include <cmath>
#include <string>

#include "qudot/quworld.h"

namespace qudot {

    R::R(int phase_k, bool dagger) : _phase_k(phase_k) {
        _id = "R" + _phase_k;
        double angle = (2 * M_PI) / (std::pow(2, _phase_k));
        if (dagger) {
            _id += dagger;
            _phase = std::conj(std::exp(QuAmp(0, static_cast<float>(angle))));
        } else {
            _phase = std::exp(QuAmp(0, static_cast<float>(angle)));
        }
    }

    std::string R::getId() const {
        return _id;
    }

    void R::applyGate(QuWorld* qw, int q) {
       int parents = qw->getRow(q);
       qw->qudot_net[parents + 1] = qw->qudot_net[parents + 1] * _phase;
       qw->qudot_net[parents + 3] = qw->qudot_net[parents + 3] * _phase;
    }

    bool operator==(const R& lhs, const R& rhs) {
        return rhs._id == lhs._id;
    }

    bool operator!=(const R& lhs, const R& rhs) {
        return !(lhs == rhs);
    }
}