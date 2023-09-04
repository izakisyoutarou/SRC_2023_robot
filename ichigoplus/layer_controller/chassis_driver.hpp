#pragma once
/*******************************************
 * chassis_driver
 * This program is base class for Chassis
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver2.0 2022/02/07 Merge into ichigoplus.
 * ver1.0 2021/01/07 The first version.
 ******************************************/

#include "ichigoplus/lib_src/two_vector.hpp"

namespace chassis_driver {

class ChassisDriver {
protected:
	using TwoVector = rp_lib::TwoVectorf;
public:
	ChassisDriver(){}
	virtual ~ChassisDriver(){}
	virtual int setup() = 0;
	virtual void cycle() = 0;
	virtual void stop() = 0;
	virtual void restart() = 0;
    virtual void calibrate() = 0;
    virtual void reset() = 0;
	virtual void move(TwoVector vel, float ang_vel) = 0;
};

}
