#pragma once

#include "ichigoplus/layer_application/console.hpp"
#include "ichigoplus/layer_application/console.hpp"
#include "ichigoplus/layer_application/cycle_once_checker.hpp"
#include "ichigoplus/layer_driver/base/can.hpp"
#include "ichigoplus/layer_driver/circuit/can_id.hpp"
#include "ichigoplus/layer_controller/motor_pos_vel_controller.hpp"
#include "ichigoplus/layer_controller/velocity_planner.hpp"
#include "ichigoplus/layer_driver/base/encoder.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"
#include <cfloat>
#include <array>

// #define CAN_ODRIVE_AXIS_ID 0x200

namespace odrive36{
struct Config{
    enum ControlMode {
        VOLTAGE_CONTROL=0,
        TORQUE_CONTROL,
        VELOCITY_CONTROL,
        POSITION_CONTROL
    };
    enum RequestedState {
        UNDEFINED=0,
        IDLE=1,
        FULL_CALIBRATION_SEQUENCE=3,
        MOTOR_CALIBRATION=4,
        ENCODER_OFFSET_CALIBRATION=7,
        CLOSED_LOOP_CONTROL=8,
        LOCKIN_SPIN=9
    };
    enum InputMode {
        INACTIVE=0,
        PASSTHROUGH=1,
        VEL_RAMP=2,
        POS_FILTER=3,
        TRAP_TRAJ=5,
        MIRROR=7,
        TUNING=8
    };

    float vel_lim = 0.f;      //許容最大速度指令(turn/s)
    float current_lim = 0.f;      //許容最大電流指令(A)
    float pos_gain = 0.f;     //位置のゲイン(N·m / (turn/s))
    float vel_gain = 0.f;     //速度のゲイン(N·m / (turn/s))
    float vel_integrator_gain = 0.f;      //速度の積分器ゲイン(N·m / (turn/s^2))
    bool pre_calibrated = false;    //事前較正を保存している場合のみTrueにする
};

class OdriveAxis : public MotorPosVelController, public CanHandler, public ConsoleCommand{
public:
    OdriveAxis(Can &can, can::CanId node_id, VelocityPlanner &velPlanner, Config &config);
    int setup() override;
	void cycle() override;

    encoder::Encoder &encoder(){return (encoder::Encoder&)encoder_; }

	void pos(float pos, float vel) override;
	void pos(float pos) override { this->pos(pos, 0.f); }
	float pos() const override { return current_pos; }
	float currentTargetPos() const override { return velPlanner.pos(); }
	void initPos(float init_pos) override;

	void vel(float vel) override;
	float vel() const override {return current_vel;};
	float currentTargetVel() const override { return velPlanner.vel(); }

	float acc() { return current_acc; }
	float duty() const override {return 0.f;}   //dutyは使わない
	void duty(float duty) override {};

    void setPosGain(float gain);
    void setVelGain(float gain, float integrator_gain);
    void setLimits(float vel, float current);

    void rev(bool rev) { is_reverse = rev; }   //回転方向変更
    void rev() {is_reverse =! is_reverse;}

	void stop() override;
	void restart() override;

    void calibration() {setAxisRequestedState(Config::RequestedState::FULL_CALIBRATION_SEQUENCE);}
    void reboot();
	void clearErrors();

    int canRead(int id,int number,unsigned char data[8])override;
	int canId(int id)override {return id == encoder_id;}
    //コンソールコマンド
    void commandName(const char* name);
	int commandCompare(char *command) override;
	int commandRequest(int argc,char **argv) override;
	int commandCycle() override;
	void consoleReadChar(char data) override;

private:
    //エンコーダ
    class Encoder : public encoder::Encoder{
    public:
        Encoder(OdriveAxis &odriveAxis) : odriveAxis(odriveAxis){};
        int setup() override {return 0;};
    	void cycle() override {};
    	void rev(bool rev) override { odriveAxis.rev(rev); };
    	float radian() const override { return odriveAxis.pos(); };
        float radianVel() const {return odriveAxis.vel(); };

    private:
        OdriveAxis &odriveAxis;
    };

	VelocityPlanner &velPlanner;
    Config &config;
    cycle_once_checker::CycleOnceChecker cycleOnceChecker;
    Encoder encoder_;


    void setAxisRequestedState(Config::RequestedState requestedState);
    void setControllerModes(Config::ControlMode controlMode, Config::InputMode inputMode);

    const can::CanId axis_id;
    const can::CanId encoder_id;
    bool can_received = false;

    bool is_reverse = false;

    float init_pos = 0.f;
	float current_pos = 0.f;
	float current_vel = 0.f;
	float current_acc = 0.f;

	rp_lib::UpEdge upedge_stop;


    enum class Mode{
		pos,
		vel,
		stay,
		stop
	} mode = Mode::stay;

    //コンソールコマンド
    char command_name[20] = "odr";
	bool sustain = false;
	const int user_command_index = 3;

	enum class CommandIndex {
		undefined,
		noMatch,
		option_error,
		help,
		getpos,
		setpos,
		getvel,
		setvel,
		getpos_gain,
		setpos_gain,
        getvel_gain,
        setvel_gain,
        getvel_int_gain,
        setvel_int_gain,
		stop,
		restart,
        calibration,
        reboot,
		clearerrors,
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
		Command("pos", 0, 1),
		Command("pos", 1, 1),
		Command("vel", 0, 1),
		Command("vel", 1, 1),
		Command("posgain", 0, 1),
		Command("posgain", 1, 1),
		Command("velgain", 0, 1),
		Command("velgain", 1, 1),
		Command("velintgain", 0, 1),
		Command("velintgain", 1, 1),
		Command("stop", 0, 0),
		Command("restart", 0, 0),
		Command("calibration", 0, 0),
		Command("reboot", 0, 0),
		Command("clearerrors", 0, 0),
	};
	char **argv = nullptr;
};
}
