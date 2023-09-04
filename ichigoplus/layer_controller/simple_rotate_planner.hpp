#pragma once
/*******************************************
 * simple_rotate_planner ver1.0 2021/03/08
 * This program plan to rotate chassis.
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver1.0 2021/03/08 The first version
 ******************************************/

#include "ichigoplus/layer_controller/rotate_planner.hpp"
#include "ichigoplus/layer_controller/velocity_planner.hpp"

namespace rotate_planner {

class SimpleRotatePlanner : public RotatePlanner {
public:
	SimpleRotatePlanner(VelocityPlanner &velocityPlanner): velocityPlanner(velocityPlanner) {}
	void cycle() { velocityPlanner.cycle(); }
	void init(float angle) { velocityPlanner.current(angle, 0.f, 0.f); }
	float pos() const override { return velocityPlanner.pos(); }
	void pos(float angle) { velocityPlanner.pos(angle); }
	float vel() const override { return velocityPlanner.vel(); }
	void vel(float ang_vel) { velocityPlanner.vel(ang_vel); }
private:
	VelocityPlanner &velocityPlanner;
};

}