#include "sm_motor_driver.hpp"

void SMMotorDriver::cycle(){
	float duty0,duty1;

	if(revFlag){
		if(duty_ >= 0.0){
			duty0 = 0.0;
			duty1 = duty_;
		}else{
			duty0 = -1.0*duty_;
			duty1 = 0.0;
		}
	}else{
		if(duty_ >= 0.0){
			duty0 = duty_;
			duty1 = 0.0;
		}else{
			duty0 = 0.0;
			duty1 = -1.0*duty_;
		}
	}

	pwm0.pwmWrite(duty0);
	pwm1.pwmWrite(duty1);
}

int SMMotorDriver::setup(){
	int i = 0;
	i += (pwm0.setupPwmOut(frequency,0.f) != 0);
	i += (pwm1.setupPwmOut(frequency,0.f) != 0);
	return i;
}