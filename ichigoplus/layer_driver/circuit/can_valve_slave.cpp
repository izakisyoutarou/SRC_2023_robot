#include "can_valve_slave.hpp"
#include <stdarg.h>


CanValveSlave::CanValveSlave(Can &can,int number,int dipNumber,Digital *d0,Digital *d1,Digital *d2,Digital *d3,Digital *d4,Digital *d5,Digital *d6,Digital *d7){
									/*  識別番号	　　ロータリーSWの値*/
	can.addHandler(this);

	this->id = number + 0x280 + dipNumber*1;////dipNumber*(mainで宣言するインスタンスの数)

	digital[0] = d0;
	digital[1] = d1;
	digital[2] = d2;
	digital[3] = d3;
	digital[4] = d4;
	digital[5] = d5;
	digital[6] = d6;
	digital[7] = d7;

	for(int pin=0; pin<8; pin++){
		Setup[pin] = CAN_DIGITAL_SETUP_NULL;
		Task[pin] = CAN_DIGITAL_SETUP_NULL;
		data[pin] = 0x00;

	}
	runSetupOnce = false;
}
int CanValveSlave::setup(){
	if(runSetupOnce==false){
		if(!canSetup()){
			runSetupOnce=true;
			canSetId(id);
			for(int pin=0; pin<8; pin++)
				digital[pin]->setupDigitalOut(),
				Setup[pin] = CAN_DIGITAL_SETUP_OUT;
			return 0;
		}else
			return 1;
	}
	return 0;
}

int CanValveSlave::canRead(int id,int number,unsigned char data[8]){

	if(number==2){
		for(int pin=0; pin<8; pin++){
			if( (data[0] >> pin) == 0x01 ){
				if(data[1]&(0x01<<pin))Task[pin]=CAN_DIGITAL_TASK_HIGH;
				else Task[pin]=CAN_DIGITAL_TASK_LOW;
			break;
			}
		}
	}
}


void CanValveSlave::cycle(){
/*
	for(int pin=0; pin<8; pin++){
			if( (0x01 << pin) & data[pin] ){//High
				Task[pin] = CAN_DIGITAL_TASK_HIGH;
			}else{						//Low
				Task[pin] = CAN_DIGITAL_TASK_LOW;
			}
	}
*/
	for(int pin=0; pin<8; pin++){
		switch(Task[pin]){

			case CAN_DIGITAL_TASK_HIGH:
				if(Setup[pin]==CAN_DIGITAL_SETUP_OUT)digital[pin]->digitalHigh();
				break;

			case CAN_DIGITAL_TASK_LOW:
				if(Setup[pin]==CAN_DIGITAL_SETUP_OUT)digital[pin]->digitalLow();
				break;

			default:
				break;
		}
		//Task[pin] = CAN_DIGITAL_TASK_NULL;
	}

}

