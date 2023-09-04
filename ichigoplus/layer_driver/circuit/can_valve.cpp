#include "can_valve.hpp"

CanValve::CanValve(Can &can,int canId,bool orderEnable){
	runSetupOnce=0;
	can.addHandler(this);

	this->canId=canId+CAN_VALVE_ID;
	pin0.canStatus(0,this);
	pin1.canStatus(1,this);
	pin2.canStatus(2,this);
	pin3.canStatus(3,this);
	pin4.canStatus(4,this);
	pin5.canStatus(5,this);
	pin6.canStatus(6,this);
	pin7.canStatus(7,this);

	this->orderFlag = orderEnable;
}

int CanValve::setup(){
	if(runSetupOnce==0){
		if(!canSetup()){
			runSetupOnce++;
			return 0;
		}else
			return 1;
	}
	return 0;
}

void CanValve::canValveTransmit(int arg,unsigned char dataArg[8]){
	canWrite(canId,arg,dataArg);
}

int CanValve::CanValvePin::setup(bool value){
	canValve->setup();

	turnOverValue=value;

	if(turnOverValue){
		status=0;
		on();
	}
	else{
		status=1;
		off();
	}

	return 0;
}

int CanValve::CanValvePin::canStatus(int number,CanValve *canValve){
	this->canValve=canValve;
	this->number=number;

	return 0;
}
void CanValve::CanValvePin::on(){
	if(!canValve->orderFlag)return;
	unsigned char data[8];
	if(turnOverValue==true)
		data[1]=0xfe;
	else
		data[1]=0x01;
	data[1]=data[1]<<(number);
	data[0]=0x01;
	data[0]=data[0]<<(number);

	if(!status)canValve->canValveTransmit(2,data);
	status=1;
}

void CanValve::CanValvePin::off(){
	if(!canValve->orderFlag)return;
	unsigned char data[8];
	if(turnOverValue==true)
		data[1]=0x01;
	else
		data[1]=0xfe;
	data[1]=data[1]<<(number);
	data[0]=0x01;
	data[0]=data[0]<<(number);

	if(status)canValve->canValveTransmit(2,data);
	status=0;
}

void CanValve::CanValvePin::toggle(){
	if(!status)
		on();
	else
		off();
}

int CanValve::CanValvePin::read(){
	return status;
}
