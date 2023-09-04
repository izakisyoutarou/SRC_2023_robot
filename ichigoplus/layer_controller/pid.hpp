#pragma once
/*******************************************
 * pid ver2.0
 * This program is pid
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver2.0 2022/02/07 Merge into ichigoplus. Change namespace
 * ver1.0 2021/03/31 The first version
 ******************************************/

#include "siso_controller.hpp"

namespace siso_controller {

class Pid : public SISOController {
public:
	Pid() = default;
	virtual ~Pid() = default;

	void virtual gain(float kp, float ki, float kd) = 0;
	float virtual kp() = 0;
	void virtual kp(float kp) { gain(kp, ki(), kd()); }
	float virtual ki() = 0;
	void virtual ki(float ki) { gain(kp(), ki, kd()); }
	float virtual kd() = 0;
	void virtual kd(float kd) { gain(kp(), ki(), kd); }
	float operator()(float current_pos, float target_pos, float target_vel, float target_acc) { return cycle(current_pos, target_pos, target_vel, target_acc); }
};

}