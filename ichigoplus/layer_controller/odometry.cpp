#include "odometry.hpp"
#include "ichigoplus/layer_driver/base/mcutime.h"
#include <stdio.h>

namespace self_localization {

int Odometry::setup() {
	int i = 0;
	if(!is_setuped) {
		i += measurementModuleX.setup() != 0;
		i += measurementModuleY.setup() != 0;
		i += gyro.setup() != 0;
		if(i == 0) is_setuped = true;
	}
	return i;
}

void Odometry::init(const Coord &coord, float ang) {
	coord_ = coord;
	initial_angle = ang - angle_;
}

void Odometry::caribrate() {
	gyro.cycle();
	base_angle = gyro.angleZ() - angle_;
	oldRealCoord = Coord(measurementModuleX.distance(angle_), measurementModuleY.distance(angle_));
	time_coord = micros();
}

void Odometry::cycle() {
	gyro.cycle();
	angle_ = gyro.angleZ() - base_angle;
	measurementModuleX.cycle();
	measurementModuleY.cycle();

	// printf("%f %f\n", measurementModuleX.distance(0.f), measurementModuleY.distance(0.f));
	
	Coord realCoord;
	realCoord = Coord(measurementModuleX.distance(angle_), measurementModuleY.distance(angle_));
	
	const Coord dRealCoord(realCoord - oldRealCoord);
	const float move_angle = angle_+initial_angle;
	const Coord dCoord(
		std::cos(measurementModuleX.measurementDirection() + move_angle) * dRealCoord.x + std::cos(measurementModuleY.measurementDirection() + move_angle) * dRealCoord.y,
		std::sin(measurementModuleX.measurementDirection() + move_angle) * dRealCoord.x + std::sin(measurementModuleY.measurementDirection() + move_angle) * dRealCoord.y
	);
	coord_ += dCoord;
	oldRealCoord = realCoord;

	const int dt = micros() - time_coord;
	time_coord = micros();
	velocity = dCoord / ((float)dt / 1000000.f);
}

}