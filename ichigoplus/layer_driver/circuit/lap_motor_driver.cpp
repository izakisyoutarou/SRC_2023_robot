#include "lap_motor_driver.hpp"

void LAPMotorDriver::cycle(){
	float duty0,duty1;
	if(revFlag){
		duty1 = (duty_ + 1.f) / 2.f;
		duty0 = 1.f - duty1;
	}
	else{
		duty0 = (duty_ + 1.f) / 2.f;
		duty1 = 1.f - duty0;
	}
	pwm0.pwmWrite(duty0);
	pwm1.pwmWrite(duty1);
}

int LAPMotorDriver::setup(){
	int i = 0;
	i += (pwm0.setupPwmOut(frequency,0.f) != 0);
	i += (pwm1.setupPwmOut(frequency,0.f) != 0);
	return i;
}
