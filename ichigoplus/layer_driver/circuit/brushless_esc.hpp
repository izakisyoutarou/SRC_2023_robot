#pragma once

#include "ichigoplus/layer_driver/base/pwm.hpp"
#include "./motor_driver.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"

class BrushlessESC{
private:
	Pwm &pwm;
	float dutyValue;
	float duty_max_limit = 1.0;
	float duty_min_limit = 0.0;
public:
	BrushlessESC(Pwm &pin):pwm(pin){};
	int setup();
	void cycle();
	void duty(float value);
	float duty(){return dutyValue;}
};
