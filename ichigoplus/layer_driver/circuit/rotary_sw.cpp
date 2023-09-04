#include "rotary_sw.hpp"

RotarySw::RotarySw(std::initializer_list<Digital*> digitals) :digitals(digitals) {
	for(int i = 0; i < digitals.size(); i++) {
		mask |= 0x01 << i;
	}
}

int RotarySw::setupInPullUp() {
	int error = 0;
	for(auto& digital : digitals) error += digital->setupDigitalInPullUp() != 0;
	return error;
}

int RotarySw::setupInPullDown() {
	int error = 0;
	for(auto& digital : digitals) error += digital->setupDigitalInPullDown() != 0;
	return error;
}

int RotarySw::setupIn() {
	int error = 0;
	for(auto& digital : digitals) error += digital->setupDigitalIn() != 0;
	return error;
}

int RotarySw::value() {
	int value = 0;
	for(int i = 0; i < digitals.size(); i++) {
		value |= digitals[i]->digitalRead() << i;
	}
	if(is_reverse) value ^= mask;
	return value;
}