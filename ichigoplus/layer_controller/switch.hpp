#pragma once
/*******************************************
 * Sw ver2.0
 * This is Switch class
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver2.1 2022/02/22 Chagne setup.
 * ver2.0 2022/02/07 Change namespace. Merge into ichigoplus.
 * ver1.0 2021/09/03 The first version.
 ******************************************/

#include "ichigoplus/layer_driver/base/digital.hpp"

namespace sw {

class Sw {
public:
	enum class SetupType { in, inPullUp, inPullDown };
	Sw(Digital &digital, SetupType setupType): digital(digital), setupType(setupType) {}
	int setup() {
		if(setupType == SetupType::in) return digital.setupDigitalIn();
		else if(setupType == SetupType::inPullUp) return digital.setupDigitalInPullUp();
		else return digital.setupDigitalInPullDown();
	}
	void rev(bool rev) { rev_flag = rev; }
	bool read() { return rev_flag^digital.digitalRead(); }
	bool operator()() { return read(); }
	
private:
	Digital &digital;
	const SetupType setupType;
	bool rev_flag = false;
};

}