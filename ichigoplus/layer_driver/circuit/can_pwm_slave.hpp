/**************************************************
 *
 **************************************************/
#pragma once
/**************************************************
 * include
 **************************************************/
#include "ichigoplus/layer_driver/base/pwm.hpp"
#include "ichigoplus/layer_driver/base/can.hpp"

/**************************************************
 * define
 **************************************************/

#define CAN_PWM_ID 0x520

class CanPwmSlave:public CanHandler{
private:
	Pwm *pwm[8];
	int number;
	int setupId,taskId,masterSetupId,masterTaskId;
	bool runSetupOnce,setupFlag;
	unsigned char Setup[8],Task[8];

	float duty[8];
	unsigned char upperDuty[8],lowerDuty[8];//上位1byte下位1byte
	unsigned char upperFrequency[8],lowerFrequency[8];//上位1byte下位1byte

	int canId(int id);
	int canRead(int id,int number,unsigned char data[8]);

	enum{
		CAN_PWM_SETUP_NULL,
		CAN_PWM_SETUP_OUT,

		CAN_PWM_TASK_NULL,
		CAN_PWM_TASK_OUT,
		CAN_PWM_TASK_READ,
	};


public:
	CanPwmSlave(Can &can,int number,int dipNumber,Pwm *p0,Pwm *p1,Pwm *p2,Pwm *p3,Pwm *p4,Pwm *p5,Pwm *p6,Pwm *p7);
	int setup();
	void cycle();

};
