#pragma once

//微分先行型PID

#include "ichigoplus/lib_src/low_pass_filter.hpp"
#include "pid.hpp"

namespace siso_controller {

class VelocityIp_d: public Pid {
private:
	using Lpf = filter::LowPassFilter<float>;
public:
	VelocityIp_d(float sampling_period_ms)
		:sampling_period(sampling_period_ms/1000.f) {}
	float cycle(float current_pos, float target_pos, float target_vel, float target_acc) override {
		x2 = x1;
		e2 = e1;
		x1 = x0;
		e1 = e0;
		x0 = lpfPos(current_pos);
		e0 = target_pos - x0;
		u += ki_ * e0 * sampling_period + kp_ * (e0 - e1) - lpfDifferential(kd_ / sampling_period * (x0- 2.f * x1 + x2));
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
	float x0 = 0.f;
	float x1 = 0.f;
	float x2 = 0.f;
	float u = 0.f;
};
}