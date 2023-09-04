#include "statusled.hpp"
#include "ichigoplus/layer_driver/base/mcutime.h"

StatusLed::StatusLed(Digital &led){
	this->led=&led;
	time = 0;
	blinkTime = 0;
	brightTime = 0;
	pulseTime = 0;
	blinkFlag = 0;
	pulseFlag = 0;
	duty = 0;
	period = 0;
	patternNum = 0;
}

int StatusLed::setup(){
	int i=0;
	if(led->setupDigitalOut())i++;
	return i;
}

void StatusLed::pulse(int pulseTime){
	if(patternNum == 0){
		this->pulseTime=pulseTime;
	}else{
		this->pulseTime=pulseTime;
		patternNum = 0;
	}
	patternNum = 1;
}

void StatusLed::blink(int period,float duty){
	if(patternNum == 0){
		this->period=period;
		this->duty=duty;
	}else{
		this->period=period;
		this->duty=duty;
		patternNum = 0;
	}
	patternNum = 2;
}

void StatusLed::on(){
	blink(1000,1);
}

void StatusLed::off(){
	blink(1000,0);
}

void StatusLed::halt(){
	blink(1000,0.5);
}

void StatusLed::manual(){
	blink(400,0.5);
}

void StatusLed::automatic(){
	blink(100,0.5);
}

void StatusLed::safety(){
	blink(500,0.1);
}

void StatusLed::disconect(){
	blink(500,0.8);
}

void StatusLed::cycle(){
	switch(patternNum){
	case 1:
		if(pulseFlag == 0){
			led->digitalHigh();
		}
		if(millis()-brightTime >= pulseTime){
			brightTime = millis();
			led->digitalLow();
			pulseFlag = 1;
		}
		break;

	case 2:
		if(millis()-time >= period){
			time = millis();
			blinkTime = millis();
			led->digitalHigh();
			blinkFlag = 1;
		}
		if(millis()-blinkTime >= duty*(float)period && blinkFlag == 1){
			led->digitalLow();
			blinkFlag = 0;
		}
		break;
	}
}
