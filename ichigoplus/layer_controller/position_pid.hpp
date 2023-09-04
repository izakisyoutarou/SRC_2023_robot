#pragma once
#include "ichigoplus/lib_src/low_pass_filter.hpp"
#include "pid.hpp"

namespace siso_controller {

class PositionPid: public Pid {
private:
    using Lpf = filter::LowPassFilter<float>;
public:
	PositionPid(float sampling_time_ms = 2.f)
        :sampling_time(sampling_time_ms/1000.f) {}
	float cycle(float current_pos, float target_pos, float target_vel, float target_acc) override {
        const float error = target_pos - lpfPos(current_pos);
        error_ = error;
        integral += error * sampling_time;
        const float operation = error*kp_ + integral*ki_ + lpfDifferential((error-old_error)/sampling_time*kd_);
        old_error = error;
        return operation;
    }
    void lpf(float constant_time_pos_ms, float constant_time_differential_ms) {
        lpfPos.configure(sampling_time*1000.f, constant_time_pos_ms);
        lpfDifferential.configure(sampling_time*1000.f, constant_time_differential_ms);
    }
	void gain(float kp, float ki, float kd) override{
        kp_ = kp;
        ki_ = ki;
        kd_ = kd;
    }
	void reset() override {
        integral = 0.f;
        old_error = 0.f;
        lpfPos.reset();
        lpfDifferential.reset();
    };

	float kp() override { return kp_; }
	float ki() override { return ki_; }
	float kd() override { return kd_; }
	float error(){ return error_; }

private:
	const float sampling_time;
	Lpf lpfPos, lpfDifferential;

	float kp_ = 0.f;
	float ki_ = 0.f;
	float kd_ = 0.f;
	float error_ = 0.f;

	float integral = 0.f;
	float old_error = 0.f;
    float sampling_period = 0.002f;
};

}
