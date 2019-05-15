#ifndef __QUDOT_QUDOTCONFIG_H
#define __QUDOT_QUDOTCONFIG_H

/**
 * Configuration object for the QuDotVM.
 * Has default parameter for the call stack size
 *
 * @see QuDotVM
 * @since 0.1.0
 */
namespace qudot {
    class QuDotConfig {
        private:
            static const unsigned int DEFAULT_STACK_SIZE = 1000;
            static const unsigned int DEFAULT_MULITVERSE_SIZE = 512;

            unsigned int num_qubits;
            unsigned int ensemble_size;
            unsigned int stack_size;
            // prints probability distribution to console
            bool print_results;
            // print worlds and qudot_nets to file
            bool print_worlds;
            unsigned int multiverse_size;

        public:
            QuDotConfig();
            unsigned int getNumQubits() const;
            void setNumQubits(const unsigned int);
            unsigned int getEnsembleSize() const;
            void setEnsembleSize(const unsigned int);
            unsigned int getStackSize() const;
            void setStackSize(const unsigned int);
            bool getPrintResults() const;
            void setPrintResults(const bool);
            bool getPrintWorlds() const;
            void setPrintWorlds(const bool);
            unsigned int getMultiverseSize() const;
            void setMultiverseSize(const unsigned int);
    };
}
#endif