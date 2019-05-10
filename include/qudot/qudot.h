#ifndef __QUDOT_QUDOT_H
#define __QUDOT_QUDOT_H

#include "qudot/qufrequency.h"

/**
 * Interface for any Virtual Machine that implements the QuDot Instruction Set
 **/
namespace qudot {
    class QuDot {
        public:
            virtual ~QuDot() {};
            virtual void bohr() = 0;
            virtual void getResults(QuFrequency&) = 0;
            virtual unsigned int getEnsemble() const = 0;
    };
}

#endif