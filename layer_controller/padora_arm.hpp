#pragma once

#include "ichigoplus/layer_controller/brush_motor_pos_vel_controller.hpp"
#include "ichigoplus/layer_controller/photo_interrputer.hpp"

class Padora_arm {
using PhotoInt = photo_interrputer::PhotoInterrputer;
public:
	Padora_arm(BrushMotorPosVelController &mc, PhotoInt &base_photo) :mc(mc), base_photo(base_photo){};

	int setup();
	void stop();
	void calibrate();
	void setting_move_top();
	void setting_move_bottom();
	void setting_move_middle();
	void setting_move_control();
	void move();
	void move_stop() {move_flag = false;};
	void manual();
	void manual_move();
	void cycle();
	bool calibrate_flag();

private:
	BrushMotorPosVelController &mc;
	PhotoInt &base_photo;
	enum class Mode {
		setup,
		manual,
		move_pos
	} mode = Mode::setup;
	Timer timer;
	int progress = 0;
	float pos_target = 0.f;
	float duty_target = 0.f;
	bool calibration_flag = false;
	bool is_calibrated = false;
	bool move_flag = false;
};
