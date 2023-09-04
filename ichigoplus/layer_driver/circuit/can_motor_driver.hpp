#pragma once
#include "motor_driver.hpp"
#include "ichigoplus/layer_driver/base/can.hpp"
#include "ichigoplus/layer_driver/base/mcutime.h"
#include <cmath>

#define CAN_MOTOR_DUTY 0x100
#define CAN_MOTOR_FREE 0x120
#define CAN_MOTOR_CURRENT_LIMIT 0x1A0

class CanMotorDriver:public MotorDriver,public CanHandler{
public:
	CanMotorDriver(Can &can,int canMdid=0);
	CanMotorDriver(Can &can,int canMdid,int limitMode,float value);
	int setup();
	void cycle();
	void outRev(bool);
	void outRev(){turnOver=!turnOver;}
	void duty(float value);
	void free(){canMdId=mdId+CAN_MOTOR_FREE;};
	float duty(){
		return outDuty/resolution();
	};
	float resolution(){return (7*std::pow(16.f,3.f)+15*std::pow(16.f,2.f)+15*16+15);};
	float currentLimit(int limitMode,float value);
	int canRead(int id,int number,unsigned char data[8]){return 0;};
	enum{
		LIMIT_AVG,
		LIMIT_PEAK,
	};
//private:
	bool turnOver;
	bool runSetupOnce;
	bool cw;
	bool startFlag;
	int freeTimer;
	int canMdId;
	int mdId;
	float outDuty = 0.f;
	float dutyValue;
	float limitValue;
	unsigned char oldData[2];
	int64_t time;
};

