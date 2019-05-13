#ifndef __QUDOT_FENWICKTREE_H
#define __QUDOT_FENWICKTREE_H

#include "qudot/common.h"

#include <vector>
#include <utility>

namespace qudot {

template <typename T>
class FenwickTree {
    private:
        T _tolerance;
        std::vector<T> _tree;
        std::vector<std::pair<size_t, T>> _worlds;

        size_t findIndexHelper(const T, size_t, size_t) const;
        
    public:
        FenwickTree(size_t, T tolerance);
        T getSum(size_t) const;
        void update(size_t, const std::pair<size_t, T>);
        size_t findIndex(const T) const; 
        const std::pair<size_t, T>& findWorld(const T) const;       
};

template <typename T>
size_t FenwickTree<T>::findIndexHelper(const T val, size_t low_index, size_t high_index) const {
    T low_sum = getSum(low_index);
    T high_sum = getSum(high_index);
    if (val > high_sum) {
        // this could be an issue but not likely: if many small probabilities are added up sometimes the
        // max probability is not 1 but something like .97xxx the below return handles that case
        return high_index-1;
    }
    if (low_index == high_index || (low_sum - _tolerance <= val && val <= high_sum + _tolerance && high_index - low_index == 1)) {
        return high_index;
    } else {
        size_t mp = (low_index + high_index) / 2;
        T mp_sum = getSum(mp);
        if (val > 0 && val <= mp_sum) {
            return findIndexHelper(val, low_index, mp);
        } else {
            return findIndexHelper(val, mp, high_index);
        }
    }    
}

template <typename T>
FenwickTree<T>::FenwickTree(size_t initial_size, T tolerance) :  _tolerance(tolerance),_tree(initial_size+1), _worlds(initial_size+1) {}

template <typename T>
T FenwickTree<T>::getSum(size_t index) const {
    T sum = 0;
    if (index < _tree.size() - 1) {
        index = index + 1;
    }

    while(index > 0) {
        sum += _tree[index];
        // index goes to parent
        index -= index & -index;
    }
    return sum;    
}

template <typename T>
void FenwickTree<T>::update(size_t index, const std::pair<size_t, T> world) {
    index = index + 1;
    _worlds[index] = world;
    while(index < _tree.size()) {
        _tree[index] += world.second;
        // index goes to child
        index += index & (-index);
    }    
}

template <typename T>
size_t FenwickTree<T>::findIndex(const T val) const {
    return findIndexHelper(val, 0, _tree.size() - 1);
}

template <typename T> 
const std::pair<size_t, T>& FenwickTree<T>::findWorld(const T val) const {
    return _worlds[findIndex(val)+1];
} 

}

#endif