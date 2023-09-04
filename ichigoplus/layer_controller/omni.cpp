#include "omni.hpp"
#include <cmath>

namespace four_omni {

int Omni::setup() {
    int i = 0;
    auto func = [&](int n){i += n != 0;};
    func(mc.setup());
    return i;
}

void Omni::cycle() {
    mc.vel(move_vel + ang_vel);
    mc.cycle();
}

void Omni::vel(float move_direction, float odometry_angle, float robo_move_vel, float robo_ang_vel) {
    moveVel(move_direction, odometry_angle, robo_move_vel);
    angVel(robo_ang_vel);
}

void Omni::moveVel(float move_direction, float odometry_angle, float robo_move_vel) {
    move_vel = robo_move_vel * std::sin(move_direction - odometry_angle - attached_direction) / wheel_radius;
}

void Omni::angVel(float robo_ang_vel) {
    ang_vel = robo_ang_vel * attached_radius / wheel_radius;
}

}