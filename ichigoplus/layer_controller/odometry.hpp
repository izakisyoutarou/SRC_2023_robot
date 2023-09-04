#pragma once
/*******************************************
 * odometry ver2.0 2022/02/06
 * This program is odometry
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver2.1 2022/02/06 Merge RC21_robot_lib into ichigoplus
 * ver2.0 2022/02/06 Support new Encoder class. Refactor program.
 * ver1.0 2021/01/12 The first version
 ******************************************/

#include "self_localization.hpp"
#include "ichigoplus/layer_driver/base/encoder.hpp"
#include "ichigoplus/layer_driver/base/gyro_sensor.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"
#include <stdint.h>

namespace self_localization {

class MeasurementModule {
public:
	// ロボットの自転で計測輪が回転する場合は, attached_radius_ms を設定し, 回転しない場合は設定しないこと.
	MeasurementModule(encoder::Encoder &enc, float wheel_radius_mm, float attached_direction, float attached_radius_mm)
		:enc(enc), wheel_radius(wheel_radius_mm/1000.f), measurement_direction(attached_direction + rp_lib::f_pi / 2.f), attached_radius(attached_radius_mm/1000.f) {}
	MeasurementModule(encoder::Encoder &enc, float wheel_radius_mm, float attached_direction)
		:enc(enc), wheel_radius(wheel_radius_mm/1000.f), measurement_direction(attached_direction), attached_radius(0.f) {}

	int setup() { return enc.setup(); }
	void cycle() { enc.cycle(); }
	float distance(float angle) const { return wheel_radius * enc.radian() - attached_radius * angle; }
	float measurementDirection() const { return measurement_direction; }
	
private:
	encoder::Encoder &enc;
	const float wheel_radius;
	const float measurement_direction;
	const float attached_radius;
};

class Odometry : public SelfLocalization {
public:
	Odometry(MeasurementModule &measurementModuleX, MeasurementModule &measurementModuleY, GyroSensor &gyro)
		:measurementModuleX(measurementModuleX), measurementModuleY(measurementModuleY), gyro(gyro) {}

	void init(const Coord &coord, float ang) override;
	int setup() override;
	void caribrate() override;

	void cycle() override;

	const Coord& coord() const override { return coord_; }
	float angle() const override { return angle_ + initial_angle; }
	const TwoVector& vel() const override { return velocity; }
	float angVel() const override { return gyro.angVelZ(); }

private:
	MeasurementModule &measurementModuleX;
	MeasurementModule &measurementModuleY;
	GyroSensor &gyro;
	Coord coord_;
	Coord oldRealCoord;
	TwoVector velocity;
	float angle_ = 0.f;
	float initial_angle = 0.f;
	float base_angle = 0.f;
	int64_t time_coord = 0.f;

	bool is_setuped = false;
};

}