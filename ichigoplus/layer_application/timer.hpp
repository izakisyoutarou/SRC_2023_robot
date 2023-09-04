#pragma once
/*******************************************
 * Timer ver1.2 2020/01/23
 * This program is timer
 *
 * [Note]
 * Notify that the set time has passed
 *
 * [Author]
 * Yuta Uehara
 * Keigo Shinomiya
 *
 * [Change history]
 * ver1.2 2020/01/23 Initialize variables
 * ver1.1 2019/12/07 Add auto reset by Yuta Uehara
 * ver1.0 2019/12/02 The first version by Yuta Uehara, Keigo Shinomiya
 ******************************************/
#include "ichigoplus/layer_driver/base/mcutime.h"

class Timer{
public:
	Timer(){};
	int setup(){return 0;}
	void set(int64_t time,bool auto_reset=false);
	bool check();

	void operator()(int64_t time,bool auto_reset=false){set(time);this->auto_reset=auto_reset;}
	bool operator()(){return check();}

private:
	int64_t settedTime=0;
	int64_t startTime=millis();
	bool auto_reset=false;
};
