#include "./brushless_esc.hpp"

int BrushlessESC::setup(){
	return pwm.setupPwmOut(60.0, 0.0);
}

void BrushlessESC::cycle(){
	dutyValue = 1.0/16.0 + dutyValue/16.0;
	pwm.pwmWrite(dutyValue);
}

void BrushlessESC::duty(float value){
	dutyValue = value;
}
