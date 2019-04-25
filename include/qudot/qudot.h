#ifndef __QUDOT_QUDOT_H
#define __QUDOT_QUDOT_H

namespace qudot {
    class QuDot {
        public:
            virtual ~QuDot() {};
            virtual void bohr() = 0;
            virtual void getResults() = 0;
    };
}

#endif