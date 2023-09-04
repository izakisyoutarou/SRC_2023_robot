#pragma once

/*******************************************
 * TrapezoidalVelocityPlanner ver2.1 2021/02/05
 * This program generate position,velocity and acceleration for trapezoidal control
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver2.1 2021/02/05 fix a bug that not work well when running vel() in continuous.
 * ver2.0 2020/11/25 rename file and class. ingertiance VelocityPlanner
 * ver1.0 2020/03/26 The first version
 ******************************************/

#include "velocity_planner.hpp"
#include <stdint.h>

namespace velocity_planner {
namespace trapezoidal_velocity_planner {
	
struct Physics_t {
	Physics_t(){}
	Physics_t(float pos, float vel, float acc):pos(pos), vel(vel), acc(acc) {}
	float pos = 0.f;
	float vel = 0.f;
	float acc = 0.f;
};

struct Limit_t {
	Limit_t(float pos, float vel, float acc, float dec): pos(pos), vel(vel), acc(acc), dec(dec) {}
	float pos = 0.f;
	float vel = 0.f;
	float acc = 0.f;
	float dec = 0.f;
};

class VelPlanner {
public:
	VelPlanner(Limit_t limit): limit_(limit) {}
	void cycle();
	void current(Physics_t physics);
	void limit(Limit_t limit) { limit_ = limit; }

	void vel(float vel);
	void vel(float vel, float start_time);
	void vel(float vel, int64_t start_time_us);

	Physics_t current() { return current_; }

	bool hasAchievedTarget() { return has_achieved_target; }

private:
	Limit_t limit_;
	Physics_t first, target, current_;

	int64_t start_time = 0;
	int64_t old_time = 0;

	float t1 = 0.f;
	float using_acc = 0.f;

	enum class Mode {
		vel,
		uniform_acceleration
	} mode = Mode::uniform_acceleration;

	bool has_achieved_target = false;
};


class PosPlanner {
public:
	PosPlanner(Limit_t limit): velPlanner(limit), limit_(limit) {}
	void cycle();
	void current(Physics_t physics);
	void limit(Limit_t limit) { limit_ = limit; velPlanner.limit(limit); }

	void pos(float pos, float vel);

	Physics_t current() { return current_; }

	bool hasAchievedTarget() { return has_achieved_target; }

private:
	VelPlanner velPlanner;
	Limit_t limit_;
	Physics_t current_, relay;

	int64_t start_time = 0;

	float relay_time = 0.f;

	float target_vel = 0.f;

	int task = 0;

	enum class Mode {
		pos,
		uniform_acceleration
	} mode = Mode::uniform_acceleration;

	bool has_achieved_target = false;
};

class TrapezoidalVelocityPlanner: public VelocityPlanner{
public:
	public:
	TrapezoidalVelocityPlanner(Limit_t limit = Limit_t(0.f, 0.f, 0.f, 0.f)): velPlanner(limit), posPlanner(limit) {}
	void cycle() override;

	void current(float pos, float vel, float acc) override;
	void current(Physics_t current) { this->current(current.pos, current.vel, current.acc); }
	Physics_t current() { return current_; }

	void limit(float pos, float vel, float acc, float dec) override;
	void limit(Limit_t limit) { this->limit(limit.pos, limit.vel, limit.acc, limit.dec); }

	void pos(float pos, float vel) override;
	void vel(float vel) override;

	float pos() override { return current_.pos; }
	float vel() override { return current_.vel; }
	float acc() override { return current_.acc; }

	bool hasAchievedTarget() override { return has_achieved_target; }

private:

	Physics_t current_;

	VelPlanner velPlanner;
	PosPlanner posPlanner;
	
	enum class Mode {
		pos,
		vel,
		uniform_acceleration,
		null
	} mode = Mode::null;

	bool has_achieved_target = false;

};

}

// 利便性のためのエイリアス及びusing宣言
using trapezoidal_velocity_planner::TrapezoidalVelocityPlanner;
using TrapezoidalVelocityPlannerLimit = trapezoidal_velocity_planner::Limit_t;

}