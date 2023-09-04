#include "filtered_encoder.hpp" //ver1.0

namespace encoder {

FilteredEncoder::FilteredEncoder(Encoder &enc)
:enc(enc) {
	enc.rev(false);
}

int FilteredEncoder::setup() {
	return enc.setup();
}

void FilteredEncoder::cycle() {
	enc.cycle();
	float radian = enc.radian();
	for(const auto& filter : filters) {
		radian = filter->filter(radian);
	}
	
	radian_ = (is_reverse ? -1.f : 1.f) * radian;
}

void FilteredEncoder::addFilter(std::initializer_list<Filter*> filters) {
	for(const auto& filter : filters) {
		addFilter(filter);
	}
}

}