#include "four_omni.hpp"
using namespace rp_lib;

namespace four_omni {

int FourOmni::setup() {
    int i = 0;
    auto func = [&](int n){i += n != 0;};
    func(omni0.setup());
    func(omni1.setup());
    func(omni2.setup());
    func(omni3.setup());
    return i;
}

void FourOmni::cycle() {
    float angle = selfLocalization.angle();
    omni0.vel(move_direction, angle, move_vel, ang_vel);
    omni1.vel(move_direction, angle, move_vel, ang_vel);
    omni2.vel(move_direction, angle, move_vel, ang_vel);
    omni3.vel(move_direction, angle, move_vel, ang_vel);
    
    float motor_vel_max = max({omni0.vel(), omni1.vel(), omni2.vel(), omni3.vel()});

    //velの大きさがmotor_vel_limitを超える場合に回転を優先しつつ、移動方向が変わらない様にする
	if(motor_vel_max > motor_vel_limit){
		float abs_ang_vel = std::abs(omni0.ang_vel);
		if(abs_ang_vel > motor_vel_limit) {
            omni0.vel(0.f, omni0.ang_vel * motor_vel_limit / abs_ang_vel);
            omni1.vel(omni0);
            omni2.vel(omni0);
            omni3.vel(omni0);
		}
		else{
			float gain = (motor_vel_limit - abs_ang_vel) / (motor_vel_max - abs_ang_vel);
            omni0.move_vel *= gain;
            omni1.move_vel *= gain;
            omni2.move_vel *= gain;
            omni3.move_vel *= gain;
		}
	}

    omni0.cycle();
    omni1.cycle();
    omni2.cycle();
    omni3.cycle();
}

void FourOmni::move(TwoVector vel, float ang_vel) {
    this->move_direction = vel.angle();
    this->move_vel = constrain(vel.length(), -move_vel_limit, move_vel_limit);
    this->ang_vel = constrain(ang_vel, -ang_vel_limit, ang_vel_limit);
}

}