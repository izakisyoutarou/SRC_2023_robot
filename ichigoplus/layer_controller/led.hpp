#pragma once
/*******************************************
 * Led ver2.0
 * This is Led class
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver2.0 2022/02/07 Change namespace. Merge into ichigoplus.
 * ver1.0 2021/09/03 The first version
 ******************************************/

#include "ichigoplus/layer_driver/base/digital.hpp"

namespace led {

class Led {
public:
	Led(Digital &digital) :digital(digital) {}
	int setup() { return digital.setupDigitalOut(); }
	void rev(bool rev) { rev_flag = rev; }
	void write(bool x) { digital.digitalWrite(rev_flag^x); }
	void on() { write(true); }
	void off() { write(false); }
	void toggle() { digital.digitalToggle(); }
	void operator()(bool x) { write(x); }
	
private:
	Digital &digital;
	bool rev_flag = false;
};

}