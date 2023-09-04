#pragma once
/*******************************************
 * simple_move_planner ver2.0
 * This program is plan to move chassis.
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
 * ver1.0 2021/03/08 The first version.
 ******************************************/

#include "move_planner.hpp"
#include "velocity_planner.hpp"

namespace move_planner {

class SimpleMovePlanner : public MovePlanner {
public:
	SimpleMovePlanner(VelocityPlanner &xVelocityPlanner, VelocityPlanner &yVelocityPlanner): xVelocityPlanner(xVelocityPlanner), yVelocityPlanner(yVelocityPlanner) {}
	void cycle() { xVelocityPlanner.cycle(); yVelocityPlanner.cycle(); }
	void init(Coord coord) { xVelocityPlanner.current(coord.x, 0.f, 0.f); yVelocityPlanner.current(coord.y, 0.f, 0.f); }
	Coord pos() const override { return Coord(xVelocityPlanner.pos(), yVelocityPlanner.pos()); }
	void pos(Coord coord) { xVelocityPlanner.pos(coord.x); yVelocityPlanner.pos(coord.y); }
	TwoVector vel() const override { return TwoVector(xVelocityPlanner.vel(), yVelocityPlanner.vel()); }
	void vel(TwoVector twoVector) { xVelocityPlanner.vel(twoVector.x); yVelocityPlanner.vel(twoVector.y); }
private:
	VelocityPlanner &xVelocityPlanner;
	VelocityPlanner &yVelocityPlanner;
};

}
