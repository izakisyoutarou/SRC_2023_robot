#pragma once
/*******************************************
 * low_pass_filter ver1.0 2022/2/4
 * This program is low-pass filter
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

namespace filter {

template <class T_I = float, class T = T_I>
class LowPassFilter : public Filter<T_I> {
public:
    LowPassFilter(T sample_time = 2.f, T constant_time = 0.f) { configure(sample_time, constant_time); }
    void configure(T sample_time, T constant_time) {
        a = sample_time / (sample_time + 2.f * constant_time);
        b = (sample_time - 2.f * constant_time) / (sample_time + 2.f * constant_time);
    }
    T_I filter(T_I value) override {
        if(is_first) {
        old_in = value;
        old_out = value;
        is_first = false;
        }
        else {
            old_out = a * (value + old_in) - b * old_out;
            old_in = value;
        }
        return old_out;
    }
    void reset() override { is_first = true; };
    T_I operator()(T_I value) { return filter(value); };

private:
    T a, b;
    T old_in, old_out;
    bool is_first = true;
};
}  // namespace filter