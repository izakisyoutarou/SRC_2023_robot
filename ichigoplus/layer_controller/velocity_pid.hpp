#pragma once
/*******************************************
 * velocity_pid ver2.0
 * This program is velocity type PID.
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver2.0 2022/02/07 Change namespace. Merge into ichigoplus.
 * ver1.0 2021/8/12 The first version
 ******************************************/

#include "ichigoplus/lib_src/low_pass_filter.hpp"
#include "pid.hpp"

namespace siso_controller {

class VelocityPid: public Pid {
private:
	using Lpf = filter::LowPassFilter<float>;
public:
	VelocityPid(float sampling_period_ms)
		:sampling_period(sampling_period_ms/1000.f){}
	float cycle(float current_pos, float target_pos, float target_vel, float target_acc) override {
		e2 = e1;
		e1 = e0;
		e0 = target_pos - lpfPos(current_pos);
		u += kp_*(e0-e1) + ki_*e0*sampling_period + lpfDifferential(kd_/sampling_period*(e0-2.f*e1+e2));
		return u;
	}
	void lpf(float constant_time_pos_ms, float constant_time_differential_ms) {
		lpfPos.configure(sampling_period*1000.f, constant_time_pos_ms);
		lpfDifferential.configure(sampling_period*1000.f, constant_time_differential_ms);
	}
	void gain(float kp, float ki, float kd) override { kp_ = kp; ki_ = ki; kd_ = kd; }
	float kp() override { return kp_; }
	float ki() override { return ki_; }
	float kd() override { return kd_; }
	void reset() override {
		e0 = 0.f;
		e1 = 0.f;
		e2 = 0.f;
		u = 0.f;
		lpfPos.reset();
		lpfDifferential.reset();
	}

private:
	Lpf lpfPos, lpfDifferential;

	float sampling_period = 0.002f;
	float sampling_period_ms = 2.f;
	float kp_ = 0.f;
	float ki_ = 0.f;
	float kd_ = 0.f;

	float e0 = 0.f;
	float e1 = 0.f;
	float e2 = 0.f;
	float u = 0.f;
};

}
