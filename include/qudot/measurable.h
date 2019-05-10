#ifndef __QUDOT_MEASURABLE_H
#define __QUDOT_MEASURABLE_H

#include <string>

namespace qudot {
    class Measurable {
        public:
            virtual ~Measurable() {};
            virtual std::string measure() = 0;
    };
}

#endif