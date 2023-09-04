#pragma once
/*******************************************
 * hampel_filter ver1.0 2022/2/4
 * This program is hampel filter
 *
 * [Dependency]
 * Filter
 *
 * [Note]
 *
 * [Author]
 * Keigo Shinomiya
 * Yuta Uehara
 *
 * [Change history]
 * ver1.0 2022/2/4 The first version
 ******************************************/

#include "filter.hpp"
#include "variance.hpp"
#include <array>
#include <algorithm>
#include <cmath>

namespace filter {

template <int window_size = 11, class T_I = float, class T = T_I>
class HampelFilter: public Filter<T_I> {
public:
    HampelFilter(T nsigma = 2.8f): nsigma(nsigma) {}
    void configure(T nsigma) { this->nsigma = nsigma; }
    T_I filter(T_I value) override {
        auto i = sortedWindow.begin();   // sortedWindow iterator.
        if (size == window_size) {
            i = std::lower_bound(sortedWindow.begin(), sortedWindow.end(), window[insert_index]);
        }
        else {
            i = sortedWindow.begin() + size;
            size++;
        }

        // Insert and sort. Almost the same as insertion sort.
        for (; i > sortedWindow.begin() && value < *(i - 1); i--) {
            *i = *(i - 1);
        }
        for (; i < sortedWindow.begin() + size - 1 && value > *(i + 1); i++) {
            *i = *(i + 1);
        }
        *i = value;

        // Calculate median
        const auto median = size % 2 == 0 ? (sortedWindow[size / 2 - 1] + sortedWindow[size / 2]) / 2.f : sortedWindow[size / 2];

        // Calculate variance.
        if(variance0.size() < window_size) {
            variance0.add(value);    
        }
        else {
            variance0.change(value, window[insert_index]);
        }
        if(variance1.size() < window_size) {
            variance1.add(value);
        }
        else {
            variance1.change(value, window[insert_index]);
        }

        // Correct variance.
        if(correctionVariance->size() == window_size) {
            const auto swap = variance;
            variance = correctionVariance;
            correctionVariance = swap;
            correctionVariance->reset();
        }
        
        // Update window
        window[insert_index] = value;
        ++insert_index %= window_size;

        // Outlier processing
        if(variance->variance() < 0 || std::abs(value - median) <= std::sqrt(variance->variance()) * nsigma) {
            return value;
        }
        else {
            return median;
        }
    }
    void reset() override {
        size = 0;
        variance0.reset();
        variance1.reset();
    }
    T_I operator()(T_I value) { return filter(value); }

private:
    std::array<T, window_size> window = {0};
    std::array<T, window_size> sortedWindow = {0};
    unsigned int insert_index = 0;
    unsigned int size = 0;
    T nsigma;
    bool is_use_variance0 = true;
    Variance<T> variance0, variance1;
    Variance<T>* variance = &variance0;
    Variance<T>* correctionVariance = &variance1;
};

}  // namespace filter
