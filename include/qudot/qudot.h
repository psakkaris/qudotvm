#ifndef __QUDOT_QUDOT_H
#define __QUDOT_QUDOT_H

/**
 * Interface for any Virtual Machine that implements the QuDot Instruction Set
 **/
namespace qudot {
    class QuDot {
        public:
            virtual ~QuDot() {};
            virtual void bohr() = 0;
            virtual void getResults() = 0;
    };
}

#endif