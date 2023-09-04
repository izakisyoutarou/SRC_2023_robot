#include "timer.hpp"

void Timer::set(int64_t time, bool auto_reset){
	settedTime = time;
	startTime = millis();
	this->auto_reset=auto_reset;
}

bool Timer::check(){
	if(millis()-startTime >= settedTime){
		if(auto_reset) startTime=millis();
		return true;
	}
	return false;
}


