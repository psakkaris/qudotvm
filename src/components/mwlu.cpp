#include "qudot/components/mwlu.h"

#include <cmath>
#include <vector>
#include <utility>

#include "boost/multiprecision/cpp_int.hpp"
#include "tbb/parallel_for.h"

#include "qudot/common.h"

namespace qudot {

void ManyWorldsLogicUnit::addModN(QuMvN* qumvn, const int val, const int N) {
    addModN(qumvn, val, N, 1, qumvn->getNumQubits());
}

void ManyWorldsLogicUnit::addModN(QuMvN* qumvn, const int val, const int N, const int start_q, const int end_q) {
    int Na = N - val;
    int mod_val = std::pow(2, end_q - start_q + 1) + val - N;

    qumvn->splitAllWorlds();

    tbb::parallel_for(qumvn->constRange(), [&] (const WorldMap::const_range_type &r) {
        for (auto it=r.begin(); it != r.end(); ++it) {
            QuWorld* quworld = it->second.get();
            comparator.compare(quworld, Na, start_q, end_q);
            full_adder.addClassicalInt(quworld, val, start_q, end_q, ONE);
            full_adder.addClassicalInt(quworld, mod_val, start_q, end_q, ZERO);
        }
    });
}

void ManyWorldsLogicUnit::ctrlAddModN(QuMvN* qumvn, const int val, const int N, const int start_q, const int end_q, const std::vector<int>& ctrls) {
    qumvn->splitWorlds(ctrls);
    tbb::parallel_for(qumvn->constRange(), [&] (const WorldMap::const_range_type &r) {
        for (auto it=r.begin(); it != r.end(); ++it) {
            QuWorld* quworld = it->second.get();
            ctrlAddModN(quworld, val, N, start_q, end_q, ctrls);              
        }
    });
}

void ManyWorldsLogicUnit::mulModN(QuMvN* qumvn, const int val, const int N) {
    mulModN(qumvn, val, N, 1, qumvn->getNumQubits());
}

void ManyWorldsLogicUnit::mulModN(QuMvN* qumvn, const int val, const int N, const int start_q, const int end_q) {
    int num_input_q = qumvn->getNumQubits();
    int n = end_q - start_q + 1;
    int other_q = num_input_q - n;
    int q = 2 * n;

    qumvn->splitAllWorlds();
    qumvn->setNumQubits((q+other_q) - num_input_q);
    tbb::parallel_for(qumvn->constRange(), [&] (const WorldMap::const_range_type &r) {
        for (auto it=r.begin(); it != r.end(); ++it) {
            QuWorld* quworld = it->second.get();
            mulModN(quworld, val, N, start_q, end_q);
        }
    });

    qumvn->setNumQubits(num_input_q);
}

void ManyWorldsLogicUnit::ctrlMulModN(QuMvN* qumvn, const int val, const int N, const int start_q, const int end_q, const std::vector<int>& ctrls) {
    qumvn->splitWorlds(ctrls);
    tbb::parallel_for(size_t(0), qumvn->getNextWorld(), [&] (size_t index) {
        QuWorld* quworld = qumvn->getQuWorld(index);
        if (quworld != nullptr) {
            ctrlMulModN(quworld, val, N, start_q, end_q, ctrls);    
        }
    });
}

//**************** PRIVATE METHODS **********************
void ManyWorldsLogicUnit::addModN(QuWorld* quworld, const int val, const int N, const int start_q, const int end_q) {
    int Na = N - val;
    int mod_val = std::pow(2, end_q - start_q + 1) + val - N;
    comparator.compare(quworld, Na, start_q, end_q);
    full_adder.addClassicalInt(quworld, val, start_q, end_q, ONE);
    full_adder.addClassicalInt(quworld, mod_val, start_q, end_q, ZERO);
}

void ManyWorldsLogicUnit::ctrlAddModN(QuWorld* quworld, const int val, const int N, const int start_q, const int end_q, const std::vector<int>& ctrls) {
    if (quworld->areActive(ctrls, ONE)) {
        addModN(quworld, val, N, start_q, end_q);
    }
}

void ManyWorldsLogicUnit::mulModN(QuWorld* quworld, const int val, const int N, const int start_q, const int end_q) {
    int num_input_q = quworld->getNumQubits();
    boost::multiprecision::cpp_int base(2);
    boost::multiprecision::cpp_int mod_bi(N);

    int n = end_q - start_q + 1;
    int other_q = num_input_q - n;
    int q = 2 * n;

    // Dimension of register above the starting Qubit
    int other_above = start_q - 1;
    // Dimension of register below the end Qubit
    int other_below = num_input_q - end_q;
    int expanded_start = n + other_above + 1;

    quworld->expandQubits((q+other_q) - num_input_q);
    for (int i=0; i < n; i++) {
        int adder_control_q = expanded_start + n - i - 1;
        boost::multiprecision::cpp_int tmp = boost::multiprecision::powm(base, i, mod_bi);
        //long tmp = baseBi.modPow(new BigInteger(Integer.toString(i)), modBi).longValue();
        int mul_modn = (tmp.convert_to<int>() * val) % N;
        ctrlAddModN(quworld, mul_modn, N, 1, n, {adder_control_q});
    }    
    std::pair<int, int> qureg_above(n + 1, n + other_above);
    std::pair<int, int> qureg_result(1, n);
    std::pair<int, int> qureg_below(quworld->getNumQubits() - other_below + 1, quworld->getNumQubits());
    quworld->contractQubits(num_input_q, qureg_above, qureg_result, qureg_below);
}

void ManyWorldsLogicUnit::ctrlMulModN(QuWorld* quworld, const int val, const int N, const int start_q, const int end_q, const std::vector<int>& ctrls) {
    if (quworld->areActive(ctrls, ONE)) {
        mulModN(quworld, val, N, start_q, end_q);
    }
}

}