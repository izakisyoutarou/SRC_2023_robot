#pragma once

/**************************************************
 * can valve ver3.3 2019/11/27
 * CanValve class.
 *
 * [Changer]
 * Yuta Uehara
 *
 * [Dependency]
 *
 * [Change history]
 * ver3.3 2019/12/02 add static_assert by Yuta Uehara
 * ver3.2 2019/11/30 bug fixes of turnOver by Yuta Uehara
 * ver3.1 2019/11/27 change program of turnOver Yuta Uehara
 * ver3.0 2019/10/20 orderEnable(false);でノード基板に命令が反映されないように変更
 * ver2.5 20??/ ?/ ? Exchange identification byte and data byte.
 * ver2.4 20??/ ?/ ? The first version.
 **************************************************/

#include "ichigoplus/layer_driver/base/can.hpp"
#include "valve.hpp"

#define CAN_VALVE_ID 0x280

class CanValve:public CanHandler{
private:
	int canId;
	bool orderFlag;
		class CanValvePin:public Valve{
			friend class CanValve;
		private:
			CanValve *canValve;
			int status;
			int number;
			int canStatus(int canId,CanValve *canValve);
		public:
			int setup(bool turnOrve=false);
			void on();
			void off();
			void toggle();
			int read();

		};

public:
	int runSetupOnce;
	CanValve(Can &can,int canId,bool orderEnable=true);
	int setup();
	void canValveTransmit(int arg,unsigned char dataArg[8]);
	int canRead(int id,int number,unsigned char data[8]){return 0;};
	void orderEnable(bool value){orderFlag=value;}
	int orderEnable(){return orderFlag;}

	CanValvePin pin0;
	CanValvePin pin1;
	CanValvePin pin2;
	CanValvePin pin3;
	CanValvePin pin4;
	CanValvePin pin5;
	CanValvePin pin6;
	CanValvePin pin7;

};
