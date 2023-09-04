

/**************************************************
 *
 **************************************************/
#pragma once
/**************************************************
 * include
 **************************************************/
#include "ichigoplus/layer_driver/base/analog.hpp"
#include "ichigoplus/layer_driver/base/can.hpp"

/**************************************************
 * define
 **************************************************/

#define CAN_ANALOG_ID 0x540

class CanAnalogSlave:public CanHandler{
private:
	Analog *analog[8];
	int number;
	int setupId,taskId,masterSetupId,masterTaskId;
	int Mode;
	bool runSetupOnce,setupFlag;
	unsigned char Setup[8],Task[8];

	int canId(int id);
	int canRead(int id,int number,unsigned char data[8]);

	enum{
		CAN_ANALOG_SETUP_NULL,
		CAN_ANALOG_SETUP_IN,

		CAN_ANALOG_TASK_NULL,
		CAN_ANALOG_TASK_READ,
	};


public:
	CanAnalogSlave(Can &can,int number,int dipNumber,Analog *a0,Analog *a1,Analog *a2,Analog *a3,Analog *a4,Analog *a5,Analog *a6,Analog *a7);
	int setup();
	void cycle();

};

