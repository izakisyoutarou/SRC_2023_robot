#pragma once
/*******************************************
 * siso_controller ver2.0
 * This program is feedback controller.
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver2.0 2022/02/07 Merge into ichigoplus. Change namespace.
 * ver1.0 2021/08/12 The first version
 ******************************************/

namespace siso_controller {

class SISOController {
public:
	SISOController() = default;
	virtual ~SISOController() = default;
	virtual void reset() = 0;
	virtual float cycle(float current_pos, float target_pos, float target_vel, float target_acc) = 0;
	float operator()(float current_pos, float target_pos, float target_vel, float target_acc) { return cycle(current_pos, target_pos, target_vel, target_acc); }
};

}