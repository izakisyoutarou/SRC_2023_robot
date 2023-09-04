#pragma once
/*******************************************
 * Omni ver1.1 2020/11/10
 * This program control omni wheel
 * 
 * [Dependency]
 * dc_motor_controller.hpp
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver2.0 2022/02/07 Merge into ichigoplus.
 * ver1.1 2020/11/10 refactoring
 * ver1.0 2020/03/09 The first version
 ******************************************/

#include "ichigoplus/layer_controller/motor_pos_vel_controller.hpp"

namespace four_omni {

class OmniLimit {
public:
    OmniLimit(float pos, float vel, float acc, float dec): pos(pos), vel(vel), acc(acc), dec(dec) {}
    OmniLimit(float pos, float vel, float acc): OmniLimit(pos, vel, acc, acc) {}
    float pos = 0;
    float vel = 0;
    float acc = 0;
    float dec = 0;
};

class Omni{
public:
    Omni(MotorPosVelController &mc, float wheel_radius, float attached_radius, float attached_direction, OmniLimit limit): limit(limit), mc(mc),wheel_radius(wheel_radius/1000.f),attached_radius(attached_radius/1000.f),attached_direction(attached_direction) {};
    int setup();
    void cycle();
    void stop(){ mc.stop(); }
    void restart(){ mc.restart(); }
    void vel(float move_direction, float odometry_angle, float robo_move_vel, float robo_ang_vel);
	void vel(float move_vel_, float ang_vel_) { move_vel = move_vel_; ang_vel = ang_vel_; }	
	void vel(const Omni &omni) { move_vel = omni.move_vel; ang_vel = omni.ang_vel; }
    void vel(float vel) { move_vel = vel; ang_vel = 0.f; }
    float vel(){ return move_vel + ang_vel; }
    void moveVel(float move_direction, float odometry_angle, float robo_move_vel);
    void angVel(float robo_ang_vel);

    float move_vel=0.f;
    float ang_vel=0.f;
    const OmniLimit limit;

private:
    MotorPosVelController &mc;
    const float wheel_radius;       //[m]
    const float attached_radius;    //[m]
    const float attached_direction;       //[rad]
};

}