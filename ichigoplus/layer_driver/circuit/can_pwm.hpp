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
#define TIMEOUT 1000//ms

class CanPwm:public CanHandler{

private:

		class CanPwmPin:public Pwm{
			friend class CanPwm;
		protected:
			int _setupPwmOut(float frequency,float duty){};
			void _pwmWrite(float duty){};
			float _pwmWrite(){};

		private:
			CanPwm *canPwm;
			int assignPinNumber(int number,CanPwm *canPwm);

			int number;
			unsigned char Setup,Task;
			float duty;


			enum{
				CAN_PWM_SETUP_NULL,
				CAN_PWM_SETUP_OUT,

				CAN_PWM_TASK_NULL,
				CAN_PWM_TASK_OUT,
				CAN_PWM_TASK_READ,
			};
		public:

			int setupPwmOut(int frequency,float duty);
			void pwmWrite(float duty);
			float pwmWrite(){return duty;}

		};
	CanPwmPin *pins[8];

	int setupId,taskId,slaveSetupId,slaveTaskId;
	int runSetupOnce;
	bool setupFlag,busCheckFlag;
	int64_t setupTimeOut;
	int sendData;

	int canRead(int id,int number,unsigned char data[8]);
	int canId(int id);

public:

	CanPwm(Can &can,int number);
	int setup();
	void cycle();

	CanPwmPin pin0;
	CanPwmPin pin1;
	CanPwmPin pin2;
	CanPwmPin pin3;
	CanPwmPin pin4;
	CanPwmPin pin5;
	CanPwmPin pin6;
	CanPwmPin pin7;

};
