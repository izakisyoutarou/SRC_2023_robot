#pragma once

/*******************************************
 * brush_motor_controller ver2.0
 * This program is ...
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver2.0 2022/02/07 Refactoring. Support new Encoder class.
 * ver1.0 2020/12/5 The first version
 ******************************************/

#include "ichigoplus/layer_driver/circuit/motor_driver.hpp"
#include "ichigoplus/layer_driver/base/encoder.hpp"
#include "siso_controller.hpp"
#include "ichigoplus/layer_application/console.hpp"
#include "motor_pos_vel_controller.hpp"
#include "velocity_planner.hpp"
#include "ichigoplus/lib_src/low_pass_filter.hpp"
#include "ichigoplus/layer_application/cycle_once_checker.hpp"
#include <cfloat>
#include <array>

class BrushMotorPosVelController : public MotorPosVelController, public ConsoleCommand {
private:
	using Encoder = encoder::Encoder;
	using SISOController = siso_controller::SISOController;
	using Lpf = filter::LowPassFilter<float>;
public:
	BrushMotorPosVelController(MotorDriver &md, Encoder &enc, VelocityPlanner &velPlanner, SISOController &sisocontroller)
		:md(md), enc(enc), velPlanner(velPlanner), sisocontroller(sisocontroller) {}

	int setup() override;
	void cycle() override;

	void pos(float pos, float vel) override;
	void pos(float pos) override { this->pos(pos, 0.f); }
	float pos() const override { return current_pos; }
	float currentTargetPos() const override { return velPlanner.pos(); }
	void initPos(float init_pos) override;

	void vel(float vel) override;
	float vel() const override {return current_vel;};
	float currentTargetVel() const override { return velPlanner.vel(); }

	float acc() { return current_acc; }
	float duty() const override { return md.duty(); }
	void duty(float duty) override;

	void stop() override;
	void restart() override;

	void outRev(bool rev) { md.outRev(rev); }
	void rotateRatio(float encoder, float target);//エンコーダと制御対象の回転数の比.
	void limit(float pos, float vel, float acc, float dec) { 									//引数はすべて正
		limit_ = Limit(pos, vel, acc, dec);
		velPlanner.limit(limit_.pos, limit_.vel, limit_.acc, limit_.dec);
	}

	void lpf(float sampling_time_ms, float constant_time_pos_ms, float constant_time_vel_ms, float constant_time_acc_ms){
		lpfPos.configure(sampling_time_ms, constant_time_pos_ms);
		lpfVel.configure(sampling_time_ms, constant_time_vel_ms);
		lpfAcc.configure(sampling_time_ms, constant_time_acc_ms);
	}

	void limitPos(float min, float max) { pos_min = min; pos_max = max; }
	void limitDuty(float min, float max) { duty_min = min; duty_max = max; }

	void samplingPeriod(float ms) { sampling_period = ms /1000.f; }

	void commandName(const char* name);
	int commandCompare(char *command) override;
	int commandRequest(int argc,char **argv) override;
	int commandCycle() override;
	void consoleReadChar(char data) override;

private:
	MotorDriver &md;
	Encoder &enc;
	VelocityPlanner &velPlanner;
	SISOController &sisocontroller;
	Lpf lpfPos, lpfVel, lpfAcc;
	cycle_once_checker::CycleOnceChecker cycleOnceChecker;

	float rotate_ratio = 1.f;

	float init_pos = 0.f;
	float current_pos = 0.f;
	float current_vel = 0.f;
	float current_acc = 0.f;
	float sampling_period = 0.002f;

	float duty_ = 0.f;

	struct Limit {
		Limit() {}
		Limit(float pos, float vel, float acc, float dec): pos(pos), vel(vel), acc(acc), dec(dec) {}
		float pos = 0.f;
		float vel = 0.f;
		float acc = 0.f;
		float dec = 0.f;
	} limit_;

	float duty_min = -0.95f;
	float duty_max = 0.95f;

	float pos_min = -FLT_MAX;
	float pos_max = FLT_MAX;

	enum class Mode{
		pos,
		vel,
		stay,
		stop,
		duty
	} mode = Mode::stay;

	char command_name[20] = "mc";
	bool sustain = false;
	const int user_command_index = 3;

	enum class CommandIndex {
		undefined,
		noMatch,
		option_error,
		help,
		enc_radian,
		getpos,
		setpos,
		getvel,
		setvel,
		getduty,
		setduty,
		stop,
		restart,
		end_index,
	} commandIndex;
	struct Command {
		Command(const char* str, int arg_num, bool repeatable): str(str), arg_num(arg_num), repeatable(repeatable) {}
		std::string str;
		int arg_num;
		bool repeatable;
		bool operator==(const Command &command) const {
			return str == command.str && arg_num == command.arg_num;
		}
	};
	const std::array<Command, (int)CommandIndex::end_index> commandList = {
		Command("undefined", 0, 0),
		Command("noMatch", 0, 0),
		Command("option_error", 0, 0),
		Command("help", 0, 0),
		Command("enc_radian", 0, 1),
		Command("pos", 0, 1),
		Command("pos", 1, 1),
		Command("vel", 0, 1),
		Command("vel", 1, 1),
		Command("duty", 0, 1),
		Command("duty", 1, 1),
		Command("stop", 0, 0),
		Command("restart", 0, 0),
	};
	char **argv = nullptr;

};
