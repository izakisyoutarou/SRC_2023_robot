/**************************************************
 *
 **************************************************/
#pragma once
/**************************************************
 * include
 **************************************************/
#include "ichigoplus/layer_driver/base/digital.hpp"
#include "ichigoplus/layer_driver/base/can.hpp"

/**************************************************
 * define
 **************************************************/

#define CAN_VALVE_ID 0x401

class CanValveSlave:public CanHandler{
private:
	Digital *digital[8];
	int number;
	int digitalarg;
	int id,masterId;
	bool runSetupOnce;
	unsigned char Setup[8],Task[8],data[8];

	int canId(int id){return id == this->id;}
	int canRead(int id,int number,unsigned char data[8]);

	enum{
		CAN_DIGITAL_SETUP_NULL,
		CAN_DIGITAL_SETUP_OUT,

		CAN_DIGITAL_TASK_NULL,
		CAN_DIGITAL_TASK_HIGH,
		CAN_DIGITAL_TASK_LOW,
	};

public:

	CanValveSlave(Can &can,int number,int dipNumber,Digital *d0,Digital *d1,Digital *d2,Digital *d3,Digital *d4,Digital *d5,Digital *d6,Digital *d7);
	int setup();
	void cycle();

};
