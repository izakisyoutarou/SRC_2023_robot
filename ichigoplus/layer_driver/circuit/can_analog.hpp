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
#define TIMEOUT 1000//ms


class CanAnalog:public CanHandler{
private:

		class CanAnalogPin:public Analog{
			friend class CanAnalog;
		protected:
			int _setupAnalogIn(){};
			float _analogRead(){};

		private:
			CanAnalog *canAnalog;
			int assignPinNumber(int number,CanAnalog *canAnalog);

			int number;
			unsigned char Setup,Task;

			float analogData;


			enum{
					CAN_ANALOG_SETUP_NULL,
					CAN_ANALOG_SETUP_IN,

					CAN_ANALOG_TASK_NULL,
					CAN_ANALOG_TASK_READ,
				};
		public:
			int analogResolution(){return 4095;}
			float analogVoltage(){return 3.3;}
			int setupAnalogIn();
			float analogRead();


		};
	int64_t setupTimeOut;
	CanAnalogPin *pins[8];

	int setupId,taskId,slaveSetupId,slaveTaskId;
	int runSetupOnce;
	bool setupFlag,busCheckFlag;

	int canId(int id);
	int canRead(int id,int number,unsigned char data[8]);

public:
	CanAnalog(Can &can,int number);
	int setup();
	void cycle();

	CanAnalogPin pin0;
	CanAnalogPin pin1;
	CanAnalogPin pin2;
	CanAnalogPin pin3;
	CanAnalogPin pin4;
	CanAnalogPin pin5;
	CanAnalogPin pin6;
	CanAnalogPin pin7;

};
