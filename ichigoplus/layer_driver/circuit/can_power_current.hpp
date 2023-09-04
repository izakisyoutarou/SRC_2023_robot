#pragma once

#include "ichigoplus/layer_driver/base/can.hpp"

#define CAN_POWER_CURRENT_ID 0x560

class CanPowerCurrent:public CanHandler{
private:
	int boardNumber;
	int canRead(int id,int number,unsigned char data[8]);
	int canId(int id);
	float value;
public:
	CanPowerCurrent(Can &can,int number);
	int setup();
	float readCurrent();
};
