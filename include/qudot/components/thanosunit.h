#ifndef __QUDOT_COMPONENTS_THANOSUNIT_H
#define __QUDOT_COMPONENTS_THANOSUNIT_H

#include <string>
#include <utility>

#include "qudot/components/feynmanunit.h"
#include "qudot/quworld.h"

namespace qudot {

    class ThanosUnit {
        private:
            FeynmanUnit feynmanUnit;

        public:
            QuWorld* createWorld(const size_t num_qubits, const size_t world_id, const size_t val);  
            std::string getQuDna(const QuWorld* quworld) const; 
            std::pair<bool, size_t> canSnap(QuWorld* quworld1, QuWorld* quworld2);
            std::pair<bool, size_t> canSnap(const std::string & qudna1, const std::string & qudna2) const;
    };

}

#endif