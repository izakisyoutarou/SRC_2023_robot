#pragma once

#include "ichigoplus/layer_driver/base/can.hpp"

#define CAN_MD_VOLTAGE_ID	0x600	//電源基板の電圧値のCANのIDは0x300

class CanMdVoltage:public CanHandler{
private:
	int boardNumber;
	int canRead(int id,int number,unsigned char data[8]);
	int canId(int id);
	float value;
public:
	CanMdVoltage(Can &can,int number=0);
	int setup();
	float read();
};
