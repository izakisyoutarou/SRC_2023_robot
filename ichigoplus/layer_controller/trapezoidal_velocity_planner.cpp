#include "trapezoidal_velocity_planner.hpp"
#include "ichigoplus/layer_driver/base/mcutime.h"
#include "ichigoplus/lib_src/utilplus.hpp"

using rp_lib::constrain;

namespace velocity_planner {
namespace trapezoidal_velocity_planner {

//VelPlanner
void VelPlanner::cycle() {
	if (mode == Mode::vel) {
        const float time = (micros() - start_time) / 1000000.f;
		if (time < t1) {
			old_time = micros();
			current_.acc = using_acc;
			current_.vel = using_acc * time + first.vel;
			current_.pos = using_acc / 2.f * time * time + first.vel * time + first.pos;
		}
		else {
			mode = Mode::uniform_acceleration;
			has_achieved_target = true;
			current_ = target;
			cycle();
		}
	}
	else if(mode == Mode::uniform_acceleration){
        const float dt = (micros() - old_time) / 1000000.f;
        old_time = micros();
		current_.pos += current_.acc / 2.f * dt * dt + current_.vel * dt;
		current_.vel += current_.acc * dt;
	}
	else {
		current_.vel = 0.f;
		current_.acc = 0.f;
	}
}

void VelPlanner::current(Physics_t physics) {
	current_ = physics;
	mode = Mode::uniform_acceleration;
	old_time = micros();
}

void VelPlanner::vel(float vel) {
    this->vel(vel, micros());
}

void VelPlanner::vel(float vel, float start_time) {
    this->vel(vel, (int64_t)(start_time * 1000000));
}

void VelPlanner::vel(float vel, int64_t start_time_us) {
	start_time = start_time_us;
	has_achieved_target = false;
	mode = Mode::vel;
	first = current_;
	target.acc = 0.f;
	target.vel = constrain(vel, -limit_.vel, limit_.vel);
	const float diff_vel = target.vel - first.vel;
	using_acc = (diff_vel >= 0) ? limit_.acc : -limit_.dec;
	t1 = diff_vel / using_acc;
	target.pos = using_acc * t1 * t1 / 2.f + first.vel * t1 + first.pos;
}


//posPlanner
void PosPlanner::cycle() {
	if (mode == Mode::pos) {
        const float time = (micros() - start_time) / 1000000.f;
		if(time >= relay_time && task == 0) {
			velPlanner.current(relay);
			velPlanner.vel(target_vel, start_time + (int64_t)(relay_time * 1000000));
			task++;
		}
		velPlanner.cycle();
		current_ = velPlanner.current();
		if(velPlanner.hasAchievedTarget() && task == 1) {
			mode = Mode::uniform_acceleration;
			has_achieved_target = true;
		}
	}
	else if (mode == Mode::uniform_acceleration) {
		velPlanner.cycle();
		current_ = velPlanner.current();
	}
	else {
		current_.vel = 0.f;
		current_.acc = 0.f;
	}
}

void PosPlanner::current(Physics_t physics) {
	mode = Mode::uniform_acceleration;
	task = 0;
	current_ = physics;
	velPlanner.current(current_);
}

void PosPlanner::pos(float pos, float vel) {
	start_time = micros();
	mode = Mode::pos;
	has_achieved_target = false;
	task = 0;
	target_vel = constrain(vel, -limit_.vel, limit_.vel);
	const float pos_diff = constrain(pos, -limit_.pos, limit_.pos) - current_.pos;

	float acc1, acc3;
	const float acc = (target_vel >= current_.vel) ? limit_.acc : -limit_.dec;
	
	if (pos_diff >= 0 && pos_diff >= (target_vel * target_vel - current_.vel * current_.vel) / (2.f * acc) 
		|| pos_diff < 0 && pos_diff > (target_vel * target_vel - current_.vel * current_.vel) / (2.f * acc) ) {
		acc1 = limit_.acc;
		acc3 = -limit_.dec;
		relay.vel = constrain(limit_.vel, 0.f, std::sqrt((acc1 * target_vel * target_vel - acc3 * current_.vel * current_.vel - 2.f * acc1 * acc3 * pos_diff) / (acc1 - acc3)));
		if(relay.vel < current_.vel) acc1 = acc3;
	}
	else {
		acc1 = -limit_.dec;
		acc3 = limit_.acc;
		relay.vel = constrain(-limit_.vel, -std::sqrt((acc1 * target_vel * target_vel - acc3 * current_.vel * current_.vel - 2.f * acc1 * acc3 * pos_diff) / (acc1 - acc3)), 0.f);
		if(relay.vel > current_.vel) acc1 = acc3;
	}

	const float x1 = (relay.vel * relay.vel - current_.vel * current_.vel) / 2.f / acc1;
	const float x3 = (target_vel * target_vel - relay.vel * relay.vel) / 2.f / acc3;
	const float x2 = pos_diff - x1 - x3;
	const float t1 = (relay.vel - current_.vel) / acc1;
	relay_time = x2 / relay.vel + t1;
	relay.pos = x1 + x2 + current_.pos;
	relay.acc = 0.f;
	velPlanner.vel(relay.vel);
}

//TrapezoidalVelocityPlanner
void TrapezoidalVelocityPlanner::current(float pos, float vel, float acc) {
	mode = Mode::uniform_acceleration;
	current_ = Physics_t(pos, vel, acc);
	velPlanner.current(current_);
}

void TrapezoidalVelocityPlanner::limit(float pos, float vel, float acc, float dec) {
	auto limit_ = Limit_t(pos, vel, acc, dec);
	velPlanner.limit(limit_);
	posPlanner.limit(limit_);
}

void TrapezoidalVelocityPlanner::pos(float pos, float vel) {
	if(mode == Mode::vel || mode == Mode::uniform_acceleration || mode == Mode::null) {
		velPlanner.cycle();
		posPlanner.current(velPlanner.current());
	}
	else {
		posPlanner.cycle();
	}
	mode = Mode::pos;
	has_achieved_target = false;
	posPlanner.pos(pos, vel);
}

void TrapezoidalVelocityPlanner::vel(float vel) { 
	if(mode == Mode::pos || mode == Mode::null) {
		posPlanner.cycle();
		velPlanner.current(posPlanner.current());
	}
	else {
		velPlanner.cycle();
	}
	mode = Mode::vel;
	has_achieved_target = false;
	velPlanner.vel(vel);
}

void TrapezoidalVelocityPlanner::cycle() {
	if (mode == Mode::pos) {
		posPlanner.cycle();
		current_ = posPlanner.current();
		if(posPlanner.hasAchievedTarget()) {
			mode = Mode::uniform_acceleration;
			velPlanner.current(current_);
			has_achieved_target = true;
		}
	}
	else if (mode == Mode::vel) {
		velPlanner.cycle();
		current_ = velPlanner.current();
		if(velPlanner.hasAchievedTarget()) {
			mode = Mode::uniform_acceleration;
			has_achieved_target = true;
		}
	}
	else if (mode == Mode::uniform_acceleration) {
		velPlanner.cycle();
		current_ = velPlanner.current();
	}
	else {
		current_.vel = 0.f;
		current_.acc = 0.f;
	}
}

}
}