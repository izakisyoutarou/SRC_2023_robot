#pragma once
#include "ichigoplus/layer_driver/base/mcutime.h"
/****************************************************************************************************************************
 * cycle_checker ver1.2 2020/11/07
 * This class detect cycle delay.
 *
 * [Note]
 *
 * [Change history]
 * ver1.2 2020/11/07 delete include util
 * ver1.1 2020/01/07 add static_assert
 * ver1.0 2019/11/02 first version by Uehara Yuta
 ***************************************************************************************************************************/
class CycleChecker{
public:
	CycleChecker(int cycle_ms);
	int setup(){return 0;}
	void cycle();
	bool operator()(){return delayFlag;}
	int64_t getDiff(){return timeDiff;}
	int64_t getMaxDelay(){return maxDelay;}
	void reset(){delayFlag = false; maxDelay = 0;}
private:
	int64_t timeDiff;
	int64_t oldTime;
	int64_t maxDelay;
	int stdCycleTime;
	bool delayFlag;
};
