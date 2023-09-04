#include "can_analog.hpp"
#include "ichigoplus/layer_driver/base/mcutime.h"

CanAnalog::CanAnalog(Can &can,int number){
	can.addHandler(this);

	this->taskId = CAN_ANALOG_ID + number*2;
	this->setupId = taskId | 0x080;
	this->slaveTaskId = CAN_ANALOG_ID + (number*2+1);
	this->slaveSetupId = slaveTaskId | 0x080;

	pin0.assignPinNumber(0,this);
	pin1.assignPinNumber(1,this);
	pin2.assignPinNumber(2,this);
	pin3.assignPinNumber(3,this);
	pin4.assignPinNumber(4,this);
	pin5.assignPinNumber(5,this);
	pin6.assignPinNumber(6,this);
	pin7.assignPinNumber(7,this);

	pins[0] = &pin0;
	pins[1] = &pin1;
	pins[2] = &pin2;
	pins[3] = &pin3;
	pins[4] = &pin4;
	pins[5] = &pin5;
	pins[6] = &pin6;
	pins[7] = &pin7;

	runSetupOnce = 0;
	busCheckFlag = setupFlag = false;
}


int CanAnalog::setup(){
	if(runSetupOnce==1)return 0;
	if(runSetupOnce==2)return 1;

	if(runSetupOnce==0){
		if(!canSetup()){

			canSetId(taskId);
			canSetId(setupId);
			unsigned char dummy[1]={0};
			canWrite(slaveSetupId,1,dummy);//通信バス確認

			setupTimeOut = millis();
			while(!busCheckFlag && millis()-setupTimeOut < TIMEOUT);

			if(busCheckFlag){
				runSetupOnce=1;
				return 0;
			}else{
				runSetupOnce=2;
				return 1;
			}
		}
	}

}

int CanAnalog::canId(int id){
	if( id == setupId)setupFlag=true;
	else setupFlag=false;
	return ( id & 0xf7f) == this->taskId;
}


int CanAnalog::canRead(int id,int number,unsigned char data[8]){

	if(setupFlag){
		if(number==1){
			busCheckFlag=true;
			setupFlag=false;
			return 0;
		}

	}else{
		int i=0;
		for(int pin=0; pin<8; pin++){

			if( (0x01 << pin) & data[0] ){
				i++;
				pins[pin]->analogData=( (float)(data[i*2-1] << 8) + (float)data[i*2] ) / 65535.0;
				if(i==3)return 0;
			}
		}
	}
}

void CanAnalog::cycle(){
	unsigned char data[8]={0}; //0:outPin

	for(int pin=0; pin<8; pin++){

		switch(pins[pin]->Task){
		case pins[pin]->CAN_ANALOG_TASK_READ:

			data[0] |= 0x01 << ( pins[pin]->number );
			break;

		default:
			break;
		}
		pins[pin]->Task = pins[pin]->CAN_ANALOG_TASK_NULL;
	}

	if(data[0] != 0x00)canWrite(slaveTaskId,1,data);
}


int  CanAnalog::CanAnalogPin::assignPinNumber(int number,CanAnalog *canAnalog){
	this->canAnalog = canAnalog;
	this->number = number;
	analogData = 0.0;
	Setup = CAN_ANALOG_SETUP_NULL;
	Task = CAN_ANALOG_TASK_NULL;

}


int CanAnalog::CanAnalogPin::setupAnalogIn(){
	if(canAnalog->setup())return 1;

	if(Setup == CAN_ANALOG_SETUP_IN)return 0;
	else if(Setup != CAN_ANALOG_SETUP_NULL)return 1;

	unsigned char data[8];
	data[0] = (0x01 << number);
	data[1] = CAN_ANALOG_SETUP_IN;

	canAnalog->canWrite(canAnalog->slaveSetupId,2,data);
	Setup = CAN_ANALOG_SETUP_IN;
	return 0;

}

float CanAnalog::CanAnalogPin::analogRead(){
	if( Setup == CAN_ANALOG_SETUP_IN ){

		Task = CAN_ANALOG_TASK_READ;
		return analogData;
	}
}
