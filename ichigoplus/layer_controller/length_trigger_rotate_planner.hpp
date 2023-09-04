#pragma once
/*******************************************
 * lengthTriggerRotatePlanner
 * This is a program for rotating a robot by following a path
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver2.1 2022/02/25 Refactor. delete unused variable and include.	
 * ver2.0 2022/02/07 Change namespace. Merge into ichigoplus.
 * ver1.0 2021/08/25 The first version
 ******************************************/

#include "velocity_planner.hpp"
#include "rotate_planner.hpp"

namespace rotate_planner {

class LengthTriggerRotatePlanner: public RotatePlanner {
public:
	LengthTriggerRotatePlanner(VelocityPlanner &velocityPlanner)
	:velocityPlanner(velocityPlanner) { }
	void cycle(float current_length) {
		if(!is_triggerd && current_length >= trigger_length) {
			velocityPlanner.pos(target_pos);
			is_triggerd = true;
		}
		velocityPlanner.cycle();
		pos_ = velocityPlanner.pos();
		vel_ = velocityPlanner.vel();
	}

	float pos() const override { return pos_; }
	float vel() const override { return vel_; }

	void init(float pos) { velocityPlanner.current(pos, 0.f, 0.f); }
	void pos(float length, float pos) {
		trigger_length = length;
		target_pos = pos;
		is_triggerd = false;
	}

private:
	VelocityPlanner &velocityPlanner;

	float pos_ = 0.f;
	float vel_ = 0.f;

	float trigger_length = 0.f;
	float target_pos = 0.f;
	bool is_triggerd = false;
};

}