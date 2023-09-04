#include "emergency.hpp"

Emergency::Emergency(Digital &emergencyPin, Digital &readPin){
	this->emergencyPin = &emergencyPin;
	this->readPin = &readPin;
	state=false;
	emergencyState=false;
}

int Emergency::setup(){
	int i=0;
	i+=emergencyPin->setupDigitalOut();
	i+=readPin->setupDigitalInPullUp();
	return i;
}

void Emergency::cycle(){
	if(state){
		emergencyPin->digitalLow();
	}
	else {
		emergencyPin->digitalHigh();
	}
	emergencyState = !(bool)readPin->digitalRead();
}

bool Emergency::readOutside(){
	return emergencyState;
}

bool Emergency::readInside(){
	return state;
}

void Emergency::on(){	//緊急停止
	state = true;
}

void Emergency::off(){	//緊急停止解除
	state = false;
}
