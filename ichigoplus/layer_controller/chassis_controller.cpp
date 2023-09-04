#include "chassis_controller.hpp" //ver1.0
#include "ichigoplus/lib_src/utilplus.hpp"

using rp_lib::constrain;
using rp_lib::rtod;

namespace chassis_controller {

int ChassisController::setup() {
	int i = 0;
	i += chassisDriver.setup() != 0;
	return i;
}

void ChassisController::cycle() {
	if(mode == Mode::trackMotion) {
		const Coord errorCoord = movePlanner.pos()-selfLocalization.coord();
		const TwoVector errorVel = movePlanner.vel()-selfLocalization.vel();
		const float errorAngle = rotatePlanner.pos()-selfLocalization.angle();
		const float errorAngVel = rotatePlanner.vel()-selfLocalization.angVel();

		if(errorVel.length() <= move_vel_tolerance && errorCoord.length() <= move_pos_tolerance) {
			moveVel_ = TwoVector(0.f, 0.f);
		}
		else {
			moveVel_ = movePlanner.vel()*move_planner_gain + errorCoord*move_pos_gain + errorVel*move_vel_gain;
			const float vel_length = moveVel_.length();
			if(vel_length > move_vel_limit) moveVel_ *= move_vel_limit / vel_length;
		}
		if(std::abs(errorAngVel) <= rotate_vel_tolerance && std::abs(errorAngle) <= rotate_pos_tolerance) {
			rotateVel_ = 0.f;
		}
		else {
			rotateVel_ = constrain(rotatePlanner.vel()*rotate_planner_gain + errorAngle*rotate_pos_gain + errorAngVel*rotate_vel_gain, -rotate_vel_limit, rotate_vel_limit);
		}

		chassisDriver.move(moveVel_, rotateVel_);
	}

	chassisDriver.cycle();
}

void ChassisController::trackMotion() {
	if(mode != Mode::stop) {
		mode = Mode::trackMotion;
	}
}

void ChassisController::vel(TwoVector moveVel, float rotate_vel ){
	if(mode != Mode::stop) {
		mode = Mode::vel;
		chassisDriver.move(moveVel, rotate_vel);
	}
}

void ChassisController::commandName(const char* name) {
	strcpy(command_name, name);
}

int ChassisController::commandCompare(char *command) {
	return !(strcmp(command_name, command));
}

int ChassisController::commandRequest(int argc, char **argv) {
	if (!strcmp(argv[1], "mgain")) {
		if (argc == 2)consolePrintf((char *)"%s mgain:%f\n", command_name, move_planner_gain);
		else if (argc == 3) {
			move_planner_gain = atoff(argv[2]);
			consolePrintf((char *)"%s mgain:%f\n", command_name, move_planner_gain);
		}
		else consolePrintf((char *)"command error\n");
	}
	else if (!strcmp(argv[1], "rgain")) {
		if (argc == 2)consolePrintf((char *)"%s rgain:%f\n", command_name, rotate_planner_gain);
		else if (argc == 3) {
			rotate_planner_gain = atoff(argv[2]);
			consolePrintf((char *)"%s rgain:%f\n", command_name, rotate_planner_gain);
		}
		else consolePrintf((char *)"command error\n");
	}
	else if (!strcmp(argv[1], "mpgain")) {
		if (argc == 2)consolePrintf((char *)"%s mpgain:%f\n", command_name, move_pos_gain);
		else if (argc == 3) {
			move_pos_gain = atoff(argv[2]);
			consolePrintf((char *)"%s mpgain:%f\n", command_name, move_pos_gain);
		}
		else consolePrintf((char *)"command error\n");
	}
	else if (!strcmp(argv[1], "mvgain")) {
		if (argc == 2)consolePrintf((char *)"%s mvgain:%f\n", command_name, move_vel_gain);
		else if (argc == 3) {
			move_vel_gain = atoff(argv[2]);
			consolePrintf((char *)"%s mvgain:%f\n", command_name, move_vel_gain);
		}
		else consolePrintf((char *)"command error\n");
	}
	else if (!strcmp(argv[1], "rpgain")) {
		if (argc == 2)consolePrintf((char *)"%s rpgain:%f\n", command_name, rotate_pos_gain);
		else if (argc == 3) {
			rotate_pos_gain = atoff(argv[2]);
			consolePrintf((char *)"%s rpgain:%f\n", command_name, rotate_pos_gain);
		}
		else consolePrintf((char *)"command error\n");
	}
	else if (!strcmp(argv[1], "rvgain")) {
		if (argc == 2)consolePrintf((char *)"%s rvgain:%f\n", command_name, rotate_vel_gain);
		else if (argc == 3) {
			rotate_vel_gain = atoff(argv[2]);
			consolePrintf((char *)"%s rvgain:%f\n", command_name, rotate_vel_gain);
		}
		else consolePrintf((char *)"command error\n");
	}
	else if (!strcmp(argv[1], "mptolerance")) {
		if (argc == 2)consolePrintf((char *)"%s mptolerance:%f\n", command_name, move_pos_tolerance);
		else if (argc == 3) {
			move_pos_tolerance = atoff(argv[2]);
			consolePrintf((char *)"%s mptolerance:%f\n", command_name, move_pos_tolerance);
		}
		else consolePrintf((char *)"command error\n");
	}
	else if (!strcmp(argv[1], "mvtolerance")) {
		if (argc == 2)consolePrintf((char *)"%s mvtolerance:%f\n", command_name, move_vel_tolerance);
		else if (argc == 3) {
			move_vel_tolerance = atoff(argv[2]);
			consolePrintf((char *)"%s mvtolerance:%f\n", command_name, move_vel_tolerance);
		}
		else consolePrintf((char *)"command error\n");
	}
	else if (!strcmp(argv[1], "rptolerance")) {
		if (argc == 2)consolePrintf((char *)"%s rptolerance:%f\n", command_name, rotate_pos_tolerance);
		else if (argc == 3) {
			rotate_pos_tolerance = atoff(argv[2]);
			consolePrintf((char *)"%s rptolerance:%f\n", command_name, rotate_pos_tolerance);
		}
		else consolePrintf((char *)"command error\n");
	}
	else if (!strcmp(argv[1], "rvtolerance")) {
		if (argc == 2)consolePrintf((char *)"%s rvtolerance:%f\n", command_name, rotate_vel_tolerance);
		else if (argc == 3) {
			rotate_vel_tolerance = atoff(argv[2]);
			consolePrintf((char *)"%s rvtolerance:%f\n", command_name, rotate_vel_tolerance);
		}
		else consolePrintf((char *)"command error\n");
	}
	else if (!strcmp(argv[1], "coord")) {
		if (argc == 2)consolePrintf((char *)"%s coord: x:%f y:%f\n", command_name, movePlanner.pos().x, movePlanner.pos().y);
		else consolePrintf((char *)"command error\n");
	}
	else if (!strcmp(argv[1], "vel")) {
		if (argc == 2)consolePrintf((char *)"%s vel: x:%f y:%f\n", command_name, movePlanner.vel().x, movePlanner.vel().y);
		else consolePrintf((char *)"command error\n");
	}
	else if (!strcmp(argv[1], "angle")) {
		if (argc == 2)consolePrintf((char *)"%s angle:%f [deg]\n", command_name, rtod(rotatePlanner.pos()));
		else consolePrintf((char *)"command error\n");
	}
	else if (!strcmp(argv[1], "angvel")) {
		if (argc == 2)consolePrintf((char *)"%s rotate_vel:%f [deg]\n", command_name, rtod(rotatePlanner.vel()));
		else consolePrintf((char *)"command error\n");
	}
	else {
		if (argc >= 2)consolePrintf((char *)"There is no \"%s\" in the command of \"%s\"\n", argv[1], command_name);
		else consolePrintf((char *)"Please enter the command of \"%s\"\n", command_name);
	}
	return 0;
}

}
