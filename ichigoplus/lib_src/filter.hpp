#pragma once
/*******************************************
 * filter ver1.0 2022/2/4
 * This program is base class for filter
 *
 * [Dependency]
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

namespace filter {

template <class T = float>
class Filter {
public:
    Filter() {};
    virtual ~Filter() = default;
    virtual T filter(T value) = 0;
    virtual void reset() = 0;
    T operator()(T value) { return filter(value); }
};

template <class T = float>
class AllPassFilter: public Filter<T> {
public:
    AllPassFilter() {};
    T filter(T value) override { return value; }
    void reset() override {}
    T operator()(T value) { return filter(value); }
};

}  // namespace filter