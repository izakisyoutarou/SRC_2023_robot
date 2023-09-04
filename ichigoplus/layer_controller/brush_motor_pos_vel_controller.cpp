#include "brush_motor_pos_vel_controller.hpp" //ver1.0
#include <string.h>
#include "ichigoplus/lib_src/utilplus.hpp"

using rp_lib::constrain;

int BrushMotorPosVelController::setup() {
	int i = 0;
	i += enc.setup() != 0;
	i += md.setup() != 0;
	return i;
}

void BrushMotorPosVelController::cycle() {
	if(!cycleOnceChecker()) return;

	enc.cycle();
	velPlanner.cycle();
	const float old_pos = current_pos;
	const float old_vel = current_vel;
	const float current_pos_raw = enc.radian() * rotate_ratio - init_pos;
	current_pos = lpfPos(current_pos_raw);
	current_vel = lpfVel(current_pos - old_pos) / sampling_period;
	current_acc = lpfVel(current_vel - old_vel) / sampling_period;

	if(mode == Mode::stop || mode == Mode::stay) {

	}
	else if(mode == Mode::duty){
		velPlanner.current(pos(), vel(), acc());
		md.duty(duty_);
		md.cycle();
	}

	else {
		float duty = sisocontroller.cycle(current_pos_raw, velPlanner.pos(), velPlanner.vel(), velPlanner.acc());
		duty = constrain(duty, duty_min, duty_max);
		md.duty(duty);
		md.cycle();
	}
}

void BrushMotorPosVelController::pos(float pos, float vel) {
	if(mode != Mode::stop) {
		mode = Mode::pos;
		pos = constrain(pos, pos_min, pos_max);
		velPlanner.pos(pos, vel);
	}
}

void BrushMotorPosVelController::initPos(float init_pos_) {
	init_pos = current_pos + init_pos - init_pos_;
	lpfPos.reset();
	current_pos = init_pos_;
	lpfPos(current_pos);
	sisocontroller.reset();

    velPlanner.current(init_pos_, velPlanner.vel(), velPlanner.acc());
    // 速度計画機の等加速度モードから少なくとも抜けるための処理
    if(mode==Mode::pos || mode==Mode::vel){
        vel(0.f);
    }
    else if(mode==Mode::duty){
        duty(0.f);
    }
}

void BrushMotorPosVelController::vel(float vel) {
	if(mode != Mode::stop) {
		mode = Mode::vel;
		velPlanner.vel(vel);
	}
}

void BrushMotorPosVelController::duty(float duty) {
	if(mode != Mode::stop){
		mode = Mode::duty;
		duty_ = constrain(duty, duty_min, duty_max);
	}
}

void BrushMotorPosVelController::stop() {
	mode = Mode::stop;
	md.duty(0.f);
	md.cycle();
}

void BrushMotorPosVelController::restart() {
	mode = Mode::stay;
	velPlanner.current(pos(), 0.f, 0.f);
	sisocontroller.reset();
	md.duty(0.f);
	md.cycle();
}

void BrushMotorPosVelController::rotateRatio(float encoder, float target) {
	rotate_ratio = target / encoder;
}

void BrushMotorPosVelController::commandName(const char* name) {
	strcpy(command_name, name);
}

int BrushMotorPosVelController::commandCompare(char *command) {
	return !(strcmp(command_name, command));
}

void BrushMotorPosVelController::consoleReadChar(char data){
	sustain = false;
}

int BrushMotorPosVelController::commandRequest(int argc, char **argv) {
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

int BrushMotorPosVelController::commandCycle() {
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
	case CommandIndex::enc_radian:
		consolePrintf((char *)"%s enc_radian->%f\n", command_name, enc.radian());
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
	case CommandIndex::getduty:
		consolePrintf((char *)"%s duty->%f\n", command_name, md.duty());
		break;
	case CommandIndex::setduty:
		duty(atoff(argv[2]));
		consolePrintf((char *)"%s duty->%f\n", command_name, duty_);
		break;
	case CommandIndex::stop:
		stop();
		consolePrintf("stoped\n");
		break;
	case CommandIndex::restart:
		restart();
		consolePrintf("restarted\n");
		break;
	default:
		consolePrintf("command error\n");
	}
	return sustain;
}
