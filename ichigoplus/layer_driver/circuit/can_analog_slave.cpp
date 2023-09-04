#include "can_analog_slave.hpp"
#include <stdarg.h>


CanAnalogSlave::CanAnalogSlave(Can &can,int number,int dipNumber,Analog *a0,Analog *a1,Analog *a2,Analog *a3,Analog *a4,Analog *a5,Analog *a6,Analog *a7){
										/*  識別番号	　　ロータリーSWの値*/
	can.addHandler(this);

	this->taskId = CAN_ANALOG_ID + (number*2+1) + dipNumber*2;//dipNumber*(mainで宣言するインスタンスの数×２)
	this->setupId = taskId | 0x080;
	this->masterTaskId = CAN_ANALOG_ID + number*2 +dipNumber*2;
	this->masterSetupId = masterTaskId | 0x080;

	analog[0] = a0;
	analog[1] = a1;
	analog[2] = a2;
	analog[3] = a3;
	analog[4] = a4;
	analog[5] = a5;
	analog[6] = a6;
	analog[7] = a7;

	for(int pin=0; pin<8; pin++){
		Setup[pin] = CAN_ANALOG_SETUP_NULL;
		Task[pin] = CAN_ANALOG_SETUP_NULL;
	}

	runSetupOnce = false;
	setupFlag = false;
}

int CanAnalogSlave::setup(){
	if(runSetupOnce==false){
		if(!canSetup()){
			runSetupOnce=true;
			canSetId(taskId);
			canSetId(setupId);
			return 0;
		}else
			return 1;
	}
	return 0;
}

int CanAnalogSlave::canId(int id){
	if( id == setupId)setupFlag=true;
	else setupFlag=false;
	return (id & 0xf7f) == this->taskId;

}

int CanAnalogSlave::canRead(int id,int number,unsigned char data[8]){

	if(setupFlag){
			if(number==1){
				unsigned char dummy[1]={0};
				canWrite(masterSetupId,1,dummy);

				return 0;
			}else{

				for(int pin=0; pin<8; pin++){
					if( (data[0] >> pin) == 0x01 ){
						Task[pin] = Setup[pin] = data[1];
						break;
					}
				}
			}
			setupFlag = false;
	}else{
		for(int pin=0; pin<8; pin++){
			if( (0x01 << pin) & data[0] && Setup[pin] == CAN_ANALOG_SETUP_IN){
				Task[pin]=CAN_ANALOG_TASK_READ;
			}else Task[pin]=CAN_ANALOG_TASK_NULL;
		}
	}
}


void CanAnalogSlave::cycle(){
//タスク
	int sendData=0;
	for(int pin=0; pin<8; pin++){

		switch(Task[pin]){
		case CAN_ANALOG_SETUP_IN:
			analog[pin]->setupAnalogIn();

		case CAN_ANALOG_TASK_READ:
			sendData++;
			break;

		default:
			break;
		}
	}
//アナログ値送信
	unsigned char data[8]={};
	int sendNumber = sendData/3 + (sendData%3>0);//送信回数
	int i=0,pin=-1;

	for(int number=1; number<=sendNumber; number++){

		for(pin=(pin+1); pin<8; pin++){

			if(Task[pin]==CAN_ANALOG_TASK_READ){
				i++;
				int analogData = analog[pin]->analogRead() * 65535;//2^16
				unsigned char upperAnalogData = (analogData & 0xff00)>>8;//上位1byte
				unsigned char lowerAnalogData =  analogData & 0x00ff;//下位1byte

				data[0]   |= (0x01 << pin);//ピン
				data[i*2-1]= upperAnalogData;
				data[i*2]  = lowerAnalogData;
				Task[pin]=CAN_ANALOG_TASK_NULL;

			}
			if(i==3){

				canWrite(masterTaskId,7,data);
				for(int i=0; i<8; i++)data[i] = 0x00;
				i=0;
				break;
			}
			if(number==sendNumber && i==sendData%3){

				canWrite(masterTaskId,i*2+1,data);
				i=4;
				break;
			}
		}
	}

}
