#ifndef __QUDOT_COMPONENTS_HEISENBERGUNIT_H
#define __QUDOT_COMPONENTS_HEISENBERGUNIT_H

#include <iostream>
#include <string>

#include "qudot/measurable.h"
#include "qudot/qufrequency.h"

/**
 * Utility class to work with measurements. Any time we want to get information about measurement about an ensemble
 * of quantum states then we use this.
 *
 * @since 0.1.0
 */

namespace qudot {
    namespace HeisenbergUnit {
        void getResults(Measurable*, unsigned int ensemble, QuFrequency&);     
        void printResults(std::ostream&, QuFrequency&);   
        void saveResults(const std::string&, QuFrequency&);
        double percentError(const double experiment, const double theory);
    }
}

#endif