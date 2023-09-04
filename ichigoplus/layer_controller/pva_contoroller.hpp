#pragma once
#include "siso_controller.hpp"

namespace siso_controller {

class PVAController: public SISOController {
public:
    PVAController(float sampling_time_ms = 2.f)
        :sampling_time(sampling_time_ms/1000.f){}

    float cycle(float current_pos, float target_pos, float target_vel, float target_acc) override {
        const float pos_error = target_pos - current_pos;
        const float current_vel = (current_pos - old_pos) / sampling_time;
		const  float vel_error = target_vel - current_vel;
        const float current_acc = (current_vel - old_vel) / sampling_time;
		const float acc_error = target_acc - current_acc;
        old_pos = current_pos;
        old_vel = current_vel;
		const float duty = pos_error * p_gain + vel_error * v_gain + acc_error * a_gain;
		return duty;
    }

    void gain(float kp, float kv, float ka) {
        p_gain = kp;
        v_gain = kv;
        a_gain = ka;
    }

    void reset() override {
        old_pos = 0.f;
        old_vel = 0.f;
    }

private:
    const float sampling_time;

    float p_gain = 0.f;
    float v_gain = 0.f;
    float a_gain = 0.f;

    float old_pos = 0.f;
    float old_vel = 0.f;
};

}