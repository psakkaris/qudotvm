#include "qudot/components/feynmanunit.h"


namespace qudot {
    FeynmanUnit::FeynmanUnit() : z(1), s(2), sdag(2, true), t(3), tdag(3, true) { }

    std::unique_ptr<R> FeynmanUnit::getRGate(int k) {
        return std::make_unique<R>(k);
    }

    std::unique_ptr<R> FeynmanUnit::getRDaggerGate(int k) {
        return std::make_unique<R>(k, true);
    }
}