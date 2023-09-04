#pragma once

#include "motor_driver.hpp"
#include "ichigoplus/layer_driver/base/pwm.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"

class LAPMotorDriver : public MotorDriver{
public:
	LAPMotorDriver(Pwm &pwm0, Pwm &pwm1, float frequency):pwm0(pwm0),pwm1(pwm1),frequency(frequency){};
	int setup() override;
	void cycle() override;
	void outRev(bool rev) override { revFlag = rev; }
	void outRev() override { outRev(!revFlag); }
	void duty(float duty) override { duty_ = rp_lib::constrain(duty, duty_min_limit, duty_max_limit); }
	float duty() override { return duty_; }
	void free() override { pwm0.pwmWrite(0.f); pwm1.pwmWrite(0.f); }
	void setDutyLimit(float min, float max) { duty_min_limit = min; duty_max_limit = max; }

private:
	Pwm &pwm0;
	Pwm &pwm1;
	const float frequency;
	float duty_max_limit = 0.95f;
	float duty_min_limit = -0.95f;
	float duty_ = 0.f;
	bool revFlag = false;
};
