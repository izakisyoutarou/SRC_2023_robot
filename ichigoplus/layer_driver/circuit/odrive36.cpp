#include "odrive36.hpp"
#include <string.h>
#include <cmath>

#define CAN_SET_AXIS_REQUESTED_STATE_ID 0x007
#define CAN_GET_ENCODER_ESTIMATES_ID 0x009
#define CAN_GET_ENCODER_COUNT_ID 0x00A
#define CAN_SET_CONTROLLER_MODES_ID 0x00B
#define CAN_SET_INPUT_POS_ID 0x00C
#define CAN_SET_INPUT_VEL_ID 0x00D
#define CAN_SET_LIMITS_ID 0x00F
#define CAN_GET_SENSORLESS_ESTIMATES 0x015
#define CAN_REBOOT_ID 0x16
#define CAN_CLEAR_ERRORS_ID 0x18
#define CAN_SET_POS_GAIN_ID 0x01A
#define CAN_SET_VEL_GAINS_ID 0x01B

using namespace rp_lib;
using namespace can;
using namespace can::can_id::odrive36;

static constexpr float stop_vel_tolerance = 0.01f;	//モータが止まったとされる速度[m/s]

namespace odrive36 {
OdriveAxis::OdriveAxis(Can &can, CanId node_id, VelocityPlanner &velPlanner, Config &config)
:velPlanner(velPlanner), config(config), encoder_(*this),
axis_id(calcId(node_id)), encoder_id(axis_id | CAN_GET_ENCODER_ESTIMATES_ID){
    can.addHandler(this);
}
int OdriveAxis::setup() {
    if(canSetup()) return 1;

    //リミッターを設定
    setLimits(config.vel_lim, config.current_lim);

    //位置ゲインを設定
    setPosGain(config.pos_gain);

    //速度ゲインを設定
    setVelGain(config.vel_gain, config.vel_integrator_gain);

    //エンコーダの推定値の受け取りの設定
    canSetId(encoder_id);

    //キャリブレーション(最初モータ，次エンコーダ)
    if(!config.pre_calibrated) calibration();

    return 0;
}

void OdriveAxis::cycle() {
    if(!cycleOnceChecker()) return;

    velPlanner.cycle();

    CanId can_id;
    unsigned char data[8] = {0,0,0,0,0,0,0,0};

	if((mode == Mode::pos || mode == Mode::vel) && upedge_stop(abs(current_vel) < stop_vel_tolerance)){
		setAxisRequestedState(Config::RequestedState::CLOSED_LOOP_CONTROL);
	}

    if(mode == Mode::pos){
        can_id = axis_id | CAN_SET_INPUT_POS_ID;
        float rotate_pos = (is_reverse ? -1 : 1) * velPlanner.pos() / (2.f * f_pi);
        float_to_uchar4(data, rotate_pos);
        canWrite(can_id,8,data);
    }
    else if(mode == Mode::vel){
        can_id = axis_id | CAN_SET_INPUT_VEL_ID;
        float rotate_vel = (is_reverse ? -1 : 1) * velPlanner.vel() / (2.f * f_pi);
        float_to_uchar4(data, rotate_vel);
        canWrite(can_id,8,data);
    }
}

void OdriveAxis::pos(float pos, float vel) {
	if(mode != Mode::stop) {
        if(mode != Mode::pos){
            setControllerModes(Config::ControlMode::POSITION_CONTROL, Config::InputMode::PASSTHROUGH);
            setAxisRequestedState(Config::RequestedState::CLOSED_LOOP_CONTROL);
        }
		mode = Mode::pos;
        //odriveの位置で制御するので位置指令に初期値を加算
		velPlanner.pos(pos+init_pos, vel);
	}
}

void OdriveAxis::initPos(float init_pos_) {
	init_pos = current_pos + init_pos - init_pos_;
}

void OdriveAxis::vel(float vel) {
	if(mode != Mode::stop) {
        if(mode != Mode::vel){
            setControllerModes(Config::ControlMode::VELOCITY_CONTROL, Config::InputMode::PASSTHROUGH);
            setAxisRequestedState(Config::RequestedState::CLOSED_LOOP_CONTROL);
        }
		mode = Mode::vel;
		velPlanner.vel(vel);
	}
}

void OdriveAxis::setPosGain(float gain) {
    CanId can_id = axis_id | CAN_SET_POS_GAIN_ID;
    unsigned char data[8];
    float_to_uchar4(data, gain);
    canWrite(can_id,4,data);
    config.pos_gain = gain;
}

void OdriveAxis::setVelGain(float gain, float integrator_gain) {
    CanId can_id = axis_id | CAN_SET_VEL_GAINS_ID;
    unsigned char data[8];
    float_to_uchar4(data, gain);
    float_to_uchar4(data+4, integrator_gain);
    canWrite(can_id,8,data);
    config.vel_gain = gain;
    config.vel_integrator_gain = integrator_gain;
}

void OdriveAxis::setLimits(float vel, float current) {
    CanId can_id = axis_id | CAN_SET_LIMITS_ID;
    unsigned char data[8];
    const float vel_turn = vel / (2.f * f_pi);  //rad/s -> turn/s
    float_to_uchar4(data, vel_turn);
    float_to_uchar4(data+4, current);
    canWrite(can_id,8,data);
    config.vel_lim = vel;
    config.current_lim = current;
}

void OdriveAxis::stop() {
	mode = Mode::stop;
    setAxisRequestedState(Config::RequestedState::IDLE);
}

void OdriveAxis::restart() {
	mode = Mode::stay;
	velPlanner.current(pos(), 0.f, 0.f);
}

void OdriveAxis::reboot() {
    CanId can_id = axis_id | CAN_REBOOT_ID;
    unsigned char data[8];
    canWrite(can_id,0,data);
}

void OdriveAxis::clearErrors() {
	CanId can_id = axis_id | CAN_CLEAR_ERRORS_ID;
	unsigned char data[8];
	canWrite(can_id,0,data);
}

//エンコーダの推定位置・速度の受け取り
int OdriveAxis::canRead(int id,int number,unsigned char data[8]){
    //回転数で制御するのでラジアンに書き換え
    //odrive側の回転方向で制御するのでメイン側の指令方向に書き換え
    current_pos = (is_reverse ? -1 : 1) * uchar4_to_float(data) * (2.f*f_pi) - init_pos;
    current_vel = (is_reverse ? -1 : 1) * uchar4_to_float(data+4) * (2.f*f_pi);
    can_received = true;
	return 0;
}

void OdriveAxis::setAxisRequestedState(Config::RequestedState requestedState){
    CanId can_id = axis_id | CAN_SET_AXIS_REQUESTED_STATE_ID;
    unsigned char data[8];
    int_to_uchar4(data, requestedState);
    canWrite(can_id,4,data);
}
void OdriveAxis::setControllerModes(Config::ControlMode controlMode, Config::InputMode inputMode){
    CanId can_id = axis_id | CAN_SET_CONTROLLER_MODES_ID;
    unsigned char data[8];
    int_to_uchar4(data, controlMode);
    int_to_uchar4(data+4, inputMode);
    canWrite(can_id,8,data);
}

//コンソールコマンド
void OdriveAxis::commandName(const char* name) {
	strcpy(command_name, name);
}

int OdriveAxis::commandCompare(char *command) {
	return !(strcmp(command_name, command));
}

void OdriveAxis::consoleReadChar(char data){
	sustain = false;
}

int OdriveAxis::commandRequest(int argc, char **argv) {
	this->argv = argv;
	if(argc >= 2) {
		commandIndex = CommandIndex::undefined;
		for(int i = user_command_index; i < (int)CommandIndex::end_index; i++) {
			if(std::string(argv[1]) == commandList[i].str) {
				const int std_arg_num = commandList[i].arg_num + 2;
				if(argc == std_arg_num) {
					commandIndex = (CommandIndex)i;
				}
				else if(argc == std_arg_num+1 && std::string(argv[std_arg_num]) == std::string("-r")) {
					if (commandList[i].repeatable) {
						commandIndex = (CommandIndex)i;
						sustain = true;
					}
					else {
						commandIndex = CommandIndex::option_error;
					}
				}
				else if(i+1 < (int)CommandIndex::end_index && commandList[i+1].str == std::string(argv[1])) {
					continue;
				}
				else {
					commandIndex = CommandIndex::noMatch;
				}
				break;
			}
		}
	}
	else {
		commandIndex = CommandIndex::help;
	}
	return commandCycle();
}

int OdriveAxis::commandCycle() {
	switch(commandIndex) {
	case CommandIndex::undefined:
		consolePrintf("%s: error. undefiend command.\n", command_name);
		break;
	case CommandIndex::noMatch:
		consolePrintf("%s: error. The number of arguments does not match.\n", command_name);
		break;
	case CommandIndex::option_error:
		consolePrintf("%s: error. can't be repeated.\n", command_name);
		break;
	case CommandIndex::help:
		consolePrintf("command list\n");
		for(int i = user_command_index; i < (int)CommandIndex::end_index; i++) {
			consolePrintf("  %s\n", commandList[i].str.c_str());
		}
		break;
	case CommandIndex::getpos:
		consolePrintf((char *)"%s pos->%f\n", command_name, pos());
		break;
	case CommandIndex::setpos:
		pos(atoff(argv[2]));
		consolePrintf((char *)"%s pos->%f\n", command_name, atoff(argv[2]));
		break;
	case CommandIndex::getvel:
		consolePrintf((char *)"%s vel->%f\n", command_name, vel());
		break;
	case CommandIndex::setvel:
		vel(atoff(argv[2]));
		consolePrintf((char *)"%s vel->%f\n", command_name, atoff(argv[2]));
		break;
	case CommandIndex::getpos_gain:
		consolePrintf((char *)"%s pos gain->%f\n", command_name, config.pos_gain);
		break;
	case CommandIndex::setpos_gain:
		setPosGain(atoff(argv[2]));
		consolePrintf((char *)"%s pos gain->%f\n", command_name, config.pos_gain);
		break;
	case CommandIndex::getvel_gain:
		consolePrintf((char *)"%s vel gain->%f\n", command_name, config.vel_gain);
		break;
	case CommandIndex::setvel_gain:
		setVelGain(atoff(argv[2]),config.vel_integrator_gain);
		consolePrintf((char *)"%s vel gain->%f\n", command_name, config.vel_gain);
		break;
	case CommandIndex::getvel_int_gain:
		consolePrintf((char *)"%s vel int gain->%f\n", command_name, config.vel_integrator_gain);
		break;
	case CommandIndex::setvel_int_gain:
		setVelGain(config.vel_gain ,atoff(argv[2]));
		consolePrintf((char *)"%s vel int gain->%f\n", command_name, config.vel_integrator_gain);
		break;
	case CommandIndex::stop:
		stop();
		consolePrintf("stoped\n");
		break;
	case CommandIndex::restart:
		restart();
		consolePrintf("restarted\n");
		break;
	case CommandIndex::calibration:
		calibration();
		consolePrintf("start calibration\n");
		break;
	case CommandIndex::reboot:
		reboot();
		consolePrintf("rebooted\n");
		break;
	case CommandIndex::clearerrors:
		clearErrors();
		consolePrintf("cleared errors\n");
		break;
	default:
		consolePrintf("command error\n");
	}
	return sustain;
}


}
