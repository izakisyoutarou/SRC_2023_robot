#pragma once

#include "ichigoplus/layer_driver/base/can.hpp"

#define CAN_MD_CURRENT_ID 0x322

class CanMdCurrent:public CanHandler{
private:
	int boardNumber;
	int canRead(int id,int number,unsigned char data[8]);
	int canId(int id);
	float value;
public:
	CanMdCurrent(Can &can,int number);
	int setup();
	float readCurrent();
};
