#pragma once
/*******************************************
 * chassis_controller
 * This program controle chassis to track path.
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver2.0 2022/02/07 Change namespace. Refactoring. Merge into ichigoplus.
 * ver1.0 2021/03/08 The first version
 ******************************************/

#include "chassis_driver.hpp"
#include "ichigoplus/layer_application/console.hpp"
#include "move_planner.hpp"
#include "rotate_planner.hpp"
#include "self_localization.hpp"

namespace chassis_controller {

class ChassisController : public ConsoleCommand {
private:
    using SelfLocalization = self_localization::SelfLocalization;
    using ChassisDriver = chassis_driver::ChassisDriver;
    using MovePlanner = move_planner::MovePlanner;
    using RotatePlanner = rotate_planner::RotatePlanner;
    using TwoVector = rp_lib::TwoVectorf;
    using Coord = coord::Coordf;

public:
    ChassisController(const SelfLocalization &selfLocalization, ChassisDriver &chassisDriver, MovePlanner &movePlanner, RotatePlanner &rotatePlanner)
        : selfLocalization(selfLocalization), chassisDriver(chassisDriver), movePlanner(movePlanner), rotatePlanner(rotatePlanner) {}
    int setup();
    void cycle();
    void stop() {
        mode = Mode::stop;
        chassisDriver.stop();
    }
    void restart() {
        mode = Mode::null;
        chassisDriver.restart();
    }

    void trackMotion();
    void vel(TwoVector moveVel, float rotate_vel);

    TwoVector& moveVel() const {return (TwoVector &)moveVel_;}
    float rotateVel(){return rotateVel_;}

	void plannerGain(float move, float rotate){
		move_planner_gain = move;
		rotate_planner_gain = rotate;
	}
    void moveGain(float pos, float vel) {
        move_pos_gain = pos;
        move_vel_gain = vel;
    }
    void rotateGain(float pos, float vel) {
        rotate_pos_gain = pos;
        rotate_vel_gain = vel;
    }
    void tolerance(float move_pos, float rotate_pos) {
        move_pos_tolerance = move_pos;
        rotate_pos_tolerance = rotate_pos;
    }
    void tolerance(float move_pos, float rotate_pos, float move_vel, float rotate_vel) {
        move_pos_tolerance = move_pos;
        rotate_pos_tolerance = rotate_pos;
        move_vel_tolerance = move_vel;
        rotate_vel_tolerance = rotate_vel;
    }
    void limit(float move_vel, float rotate_vel) {
        move_vel_limit = move_vel;
        rotate_vel_limit = rotate_vel;
    }

    void commandName(const char *command_name);
    int commandCompare(char *command) override;
    int commandRequest(int argc, char **argv) override;

private:
    const SelfLocalization &selfLocalization;
    ChassisDriver &chassisDriver;
    MovePlanner &movePlanner;
    RotatePlanner &rotatePlanner;

    TwoVector moveVel_;
    float rotateVel_ = 0.f;

    enum class Mode { null, trackMotion, vel, stop } mode = Mode::stop;

	float move_planner_gain = 0.f;
	float rotate_planner_gain = 0.f;
    float move_pos_gain = 0.f;
    float move_vel_gain = 0.f;
    float rotate_pos_gain = 0.f;
    float rotate_vel_gain = 0.f;
    float move_vel_limit = 0.f;
    float rotate_vel_limit = 0.f;

    float move_pos_tolerance = 0.f;
    float move_vel_tolerance = 0.f;
    float rotate_pos_tolerance = 0.f;
    float rotate_vel_tolerance = 0.f;

    char command_name[20] = "cc";
};

}  // namespace rc21