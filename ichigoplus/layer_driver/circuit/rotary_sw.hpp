#pragma once

#include "ichigoplus/layer_driver/base/digital.hpp"

#include <initializer_list>
#include <vector>

class RotarySw {
public:
	RotarySw(std::initializer_list<Digital*> digital);
	int setupInPullUp();
	int setupInPullDown();
	int setupIn();
	int value();
	void rev(bool rev) { is_reverse = rev; }
	int howmany() { return digitals.size(); }

private:
	std::vector<Digital*> digitals;
	int mask = 0;
	bool is_reverse = false;
};
