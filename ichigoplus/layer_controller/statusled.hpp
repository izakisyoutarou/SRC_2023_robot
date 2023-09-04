#pragma once
#include "ichigoplus/layer_driver/base/digital.hpp"

class StatusLed{
private:
	Digital *led;
	int time;
	int blinkTime;
	int brightTime;
	int pulseTime;
	int blinkFlag;
	int pulseFlag;
	float duty;
	int period;
	int patternNum;
public:
	StatusLed(Digital &ledpin);
	int setup();
	void pulse(int pulseTime);
	void blink(int period,float duty);
	void on();
	void off();
	void halt();
	void manual();
	void automatic();
	void safety();
	void disconect();
	void cycle();
};
