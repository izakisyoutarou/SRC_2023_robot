#include "can_pwm.hpp"
#include "ichigoplus/layer_driver/base/mcutime.h"
#include "ichigoplus/lib_src/utilplus.hpp"
using namespace rp_lib;

CanPwm::CanPwm(Can &can,int number){
	can.addHandler(this);

	this->taskId = CAN_PWM_ID + number*2;
	this->setupId = taskId | 0x080;
	this->slaveTaskId = CAN_PWM_ID + (number*2+1);
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
	sendData = 0;
}


int CanPwm::setup(){
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


int CanPwm::canId(int id){
	if( id == setupId)setupFlag=true;
	else setupFlag=false;
	return ( id & 0xf7f) == this->taskId;
}

int CanPwm::canRead(int id,int number,unsigned char data[8]){
	if(setupFlag){
		if(number==1){
			busCheckFlag=true;
			setupFlag=false;
			return 0;
		}
	}
}

void CanPwm::cycle(){
	unsigned char data[8];
	int sendNumber = sendData/3 + (sendData%3>0);//送信回数
	int i=0,pin=0;

	for(int number=1; number<=sendNumber; number++){

		for(pin=pin; pin<8; pin++){

			if(pins[pin]->Task==pins[pin]->CAN_PWM_TASK_OUT){
				i++;
				int duty = floatlimit(0.0,pins[pin]->duty,1.0) * 65535;//2^16
				unsigned char upperDuty = (duty & 0xff00)>>8;//上位1byte
				unsigned char lowerDuty =  duty & 0x00ff;//下位1byte

				data[0]   |= (0x01 << pins[pin]->number);//ピン
				data[i*2-1]= upperDuty;
				data[i*2]  = lowerDuty;

				pins[pin]->Task=pins[pin]->CAN_PWM_TASK_NULL;
			}
			if(i==3){
				canWrite(slaveTaskId,7,data);
				data[0] = 0x00;
				i=0;
				break;
			}
			if(number==sendNumber && i==sendData%3){
				canWrite(slaveTaskId,i*2+1,data);
				i=4;
				break;
			}
		}
	}
	sendData=0;
}


int  CanPwm::CanPwmPin::assignPinNumber(int number,CanPwm *canPwm){
	this->canPwm = canPwm;
	this->number = number;

	Setup = CAN_PWM_SETUP_NULL;
	Task = CAN_PWM_TASK_NULL;

	duty=0.0;

}

int CanPwm::CanPwmPin::setupPwmOut(int frequency,float duty){
	if(canPwm->setup())return 1;

	if(Setup == CAN_PWM_SETUP_OUT)return 0;
	else if(Setup != CAN_PWM_SETUP_NULL)return 1;


	unsigned char upperFrequency = (frequency & 0xff00)>>8;//上位1byte
	unsigned char lowerFrequency = (frequency & 0x00ff);//下位1byte

	duty = floatlimit(0.0,duty,1.0) * 65535;//2^16
	unsigned char upperDuty = ((int)duty & 0xff00)>>8;//上位1byte
	unsigned char lowerDuty = (int)duty & 0x00ff;//下位1byte


	unsigned char data[8];
	data[0] = (0x01 << number);//ピン
	data[1] = upperDuty;//上位1byte
	data[2] = lowerDuty;//下位1byte
	data[3] = upperFrequency;
	data[4] = lowerFrequency;

	canPwm->canWrite(canPwm->slaveSetupId,6,data);

	Setup = CAN_PWM_SETUP_OUT;
	return 0;

}

void CanPwm::CanPwmPin::pwmWrite(float duty){
	if(Setup==CAN_PWM_SETUP_OUT){

		this->duty=duty;
		Task = CAN_PWM_TASK_OUT;
		canPwm->sendData++;

	}
}

