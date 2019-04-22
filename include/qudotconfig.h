#ifndef __QUDOT_QUDOTCONFIG_H
#define __QUDOT_QUDOTCONFIG_H

class QuDotConfig {
    private:
         static const unsigned int DEFAULT_STACK_SIZE = 1000;
         static const unsigned int DEFAULT_MULITVERSE_SIZE = 512;

         unsigned int num_qubits;
         unsigned int ensemble_size;
         unsigned int stack_size;
         bool print_results;
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
        unsigned int getMultiverseSize() const;
        void setMultiverseSize(const unsigned int);
};
#endif