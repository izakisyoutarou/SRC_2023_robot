#pragma once
/*******************************************
 * filtered_encoder ver1.0 2022/02/04
 * This program filter encoder value.
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver1.0 2022/02/04 The first version
 ******************************************/

#include "ichigoplus/layer_driver/base/encoder.hpp"
#include "ichigoplus/lib_src/filter.hpp"
#include <vector>
#include <initializer_list>

namespace encoder {
class FilteredEncoder : public Encoder {
	using Filter = filter::Filter<float>;
public:
	FilteredEncoder(Encoder &enc);
	int setup() override;
	void cycle() override;
	void rev(bool rev) override { is_reverse = rev; }
	float radian() const override { return radian_; }
	void addFilter(Filter *filter) { filters.push_back(filter); }
	void addFilter(Filter &filter) { addFilter(&filter); }
	void addFilter(std::initializer_list<Filter*> filters);

private:
	Encoder &enc;
	std::vector<Filter*> filters;

	bool is_reverse= false;
	float radian_ = 0.f;
};
}