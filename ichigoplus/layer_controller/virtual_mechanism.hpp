#pragma once
/*******************************************
 * virtual_mechanism ver2.0
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver2.0 2022/02/07 Change namespace. Support new Encoder class. Merge into ichigoplus.
 * ver1.0 2021/8/20 The first version
 ******************************************/

#include "ichigoplus/layer_driver/circuit/motor_driver.hpp"
#include "ichigoplus/layer_driver/base/encoder.hpp"
#include "ichigoplus/layer_driver/base/digital.hpp"
#include "photo_interrputer.hpp"
#include "ichigoplus/layer_application/timer.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"
#include <queue>
#include <vector>

namespace virtual_mechanism {

class VirtualMotorDriver: public MotorDriver {
public:
	int setup() override { return 0; }
	void cycle() override { timer(10); }
	void outRev(bool rev) override {}
	void outRev() override {}
	void duty(float duty) override { duty_ = duty; }
	float duty() override { return duty_; }
	void free() override { duty_ = 0.f; }
	float outDuty() { return timer() ? 0.f : duty_; }
private:
	Timer timer;
	float duty_ = 0.f;
};

class VirtualSensor {
public:
	VirtualSensor() = default;
	virtual ~VirtualSensor() = default;
	virtual void virtualPos(float pos) = 0;
};

class VirtualEncoder: public encoder::Encoder, public VirtualSensor {
public:
	int setup() override { return 0; }
	void cycle() override { radian_ = buff; }
	void rev(bool rev) override { rev_ = rev; }
	float radian() const override { return radian_; }
	
	void virtualPos(float pos) override { buff = (pos+offset_) / convert_ratio * (rev_ ? -1.f : 1.f); }

	void offset(float offset) { offset_ = offset; }
	void convertRatio(float enc_vel, float target_vel) { convert_ratio = target_vel / enc_vel;}

private:
	float offset_ = 0.f;
	float convert_ratio = 1.f;
	bool rev_ = false;
	float radian_ = 0.f;
	float buff = 0.f;
};

class VirtualPhotoInterrupter: public photo_interrputer::PhotoInterrputer, public VirtualSensor {
public:
	VirtualPhotoInterrupter() { stopReading(); }
	
	int setup() override { return 0; }
	void startReading(int sampling_period_ms) override { timer(sampling_period_ms, true); }
	void stopReading() override { timer(1000*60*60*24, true); }
	bool read() override { return value_; }

	void lowArea(float pos_min, float pos_max) { is_high_interrupt = false; area_pos_min = pos_min; area_pos_max = pos_max; }
	void highArea(float pos_min, float pos_max) { is_high_interrupt = true; area_pos_min = pos_min; area_pos_max = pos_max; }
	void virtualPos(float pos) override { if(timer()) value_ = (pos >= area_pos_min && pos <= area_pos_max) ? is_high_interrupt : !is_high_interrupt; }

private:
	Timer timer;
	bool value_ = 0;
	float area_pos_min = 0.f;
	float area_pos_max = -1.f;
	bool is_high_interrupt = false;
};

class VirtualMechanism {
public:
	VirtualMechanism(VirtualMotorDriver &md): md(md) {
		inputDuty.push(Duty(0, 0.f));
	}
	void cycle() {
		const auto time = micros();
		const float dt = (time-old_time)/1000.f/1000.f;
		old_time = time;

		vel_ = rp_lib::constrain(duty_*max_vel, min_limit_vel, max_limit_vel);
		pos_ += vel_*dt;
		pos_ = rp_lib::constrain(pos_, min_limit_pos, max_limit_pos);
		pos(pos_);

		inputDuty.push(Duty(micros()+dead_time, md.outDuty()));
		while(!inputDuty.empty()) {
			if(inputDuty.front().apply_time <= micros()) {
				duty_ = inputDuty.front().duty;
				inputDuty.pop();
			}
			else {
				break;
			}
		}
	}
	
	float duty() { return duty_; }
	float vel() { return vel_; }
	float pos() { return pos_; }
	void pos(float pos) {
		pos_ = pos;
		for(auto &sensor_ : sensor) {
			sensor_->virtualPos(pos_);
		}		
	}

	void limitPos(float min, float max) { min_limit_pos = min; max_limit_pos = max; }
	void limitVel(float min, float max) { min_limit_vel = min; max_limit_vel = max; }
	void maxVel(float vel) { max_vel = vel; }
	void deadTime(int dead_time_us) { dead_time = dead_time_us; }
	void addSensor(VirtualSensor &sensor) { this->sensor.push_back(&sensor); sensor.virtualPos(pos_); }

private:
	VirtualMotorDriver &md;
	std::vector<VirtualSensor*> sensor;
	
	float duty_ = 0.f;
	float vel_ = 0.f;
	float pos_ = 0.f;

	float max_limit_pos = 10000.f;
	float min_limit_pos = -10000.f;
	float max_limit_vel = 10000.f;
	float min_limit_vel = -10000.f;
	float max_vel = 0.f;
	int64_t dead_time = 0;			// 無駄時間 [us]

	int64_t old_time = 0;
	struct Duty {
		Duty(int64_t apply_time, float duty): apply_time(apply_time), duty(duty) {}
		int64_t apply_time;
		float duty;
	};
	std::queue<Duty> inputDuty;
};

}