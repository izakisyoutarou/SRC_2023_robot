#pragma once

#include "ichigoplus/layer_driver/base/can.hpp"

#define CAN_POWER_VOLTAGE_ID 0x580

class CanPowerVoltage:public CanHandler{
private:
	int boardNumber;
	int canRead(int id,int number,unsigned char data[8]);
	int canId(int id);
	float value;
public:
	CanPowerVoltage(Can &can,int number=0);
	int setup();
	float readVoltage();
};
