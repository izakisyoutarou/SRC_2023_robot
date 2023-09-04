#include "cycle_checker.hpp" //ver1.2

CycleChecker::CycleChecker(int cycle_ms){
	timeDiff = 0;
	oldTime = millis();
	maxDelay = 0;
	stdCycleTime = cycle_ms;
	delayFlag = false;
}

void CycleChecker::cycle(){
	int64_t time = millis();
	timeDiff = time - oldTime;
	if(timeDiff > stdCycleTime){
		delayFlag = true;
		if(timeDiff > maxDelay){
			maxDelay = timeDiff;
		}
	}
	oldTime = time;
}
