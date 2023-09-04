#include "can_pwm_slave.hpp"

#include <stdarg.h>

CanPwmSlave::CanPwmSlave(Can &can,int number,int dipNumber,Pwm *p0,Pwm *p1,Pwm *p2,Pwm *p3,Pwm *p4,Pwm *p5,Pwm *p6,Pwm *p7){
								/*  識別番号	　　ロータリーSWの値*/
	can.addHandler(this);

	this->taskId = CAN_PWM_ID + (number*2+1) + dipNumber*2;//dipNumber*(mainで宣言するインスタンスの数×２)
	this->setupId = taskId | 0x080;
	this->masterTaskId = CAN_PWM_ID + number*2 +dipNumber*2;
	this->masterSetupId = masterTaskId | 0x080;

	pwm[0] = p0;
	pwm[1] = p1;
	pwm[2] = p2;
	pwm[3] = p3;
	pwm[4] = p4;
	pwm[5] = p5;
	pwm[6] = p6;
	pwm[7] = p7;


	for(int pin=0; pin<8; pin++){
		Setup[pin] = CAN_PWM_SETUP_NULL;
		Task[pin] = CAN_PWM_TASK_NULL;
		upperDuty[pin]=0,lowerDuty[pin]=0;
		upperFrequency[pin]=0,lowerFrequency[pin]=0;
		duty[pin] = 0.0;
	}
	runSetupOnce = false;
	setupFlag = false;

}
int CanPwmSlave::setup(){
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

int CanPwmSlave::canId(int id){
	if( id == setupId)setupFlag=true;
	else setupFlag=false;
	return ( id & 0xf7f) == this->taskId;
}

int CanPwmSlave::canRead(int id,int number,unsigned char data[8]){

	if(setupFlag){
			if(number==1){
					unsigned char dummy[1]={0};
					canWrite(masterSetupId,1,dummy);

					return 0;
			}else{

				for(int pin=0; pin<8; pin++){
					if( (data[0] >> pin) == 0x01 ){
						upperDuty[pin]=data[1];
						lowerDuty[pin]=data[2];
						upperFrequency[pin]=data[3];
						lowerFrequency[pin]=data[4];
						Task[pin] = CAN_PWM_SETUP_OUT;
						return 0;
					}
				}
			}
			setupFlag = false;
	}else{
		int i=0;
		for(int pin=0; pin<8; pin++){
			if( (0x01 << pin) & data[0] ){
				i++;
				duty[pin]=( (data[i*2-1] << 8) + data[i*2] ) / 65535.0;
				Task[pin] = CAN_PWM_TASK_OUT;
				if(i==3)return 0;
			}
		}
	}
}


void CanPwmSlave::cycle(){

	for(int pin=0; pin<8; pin++){

		switch(Task[pin]){

		case CAN_PWM_SETUP_OUT:
		{
			float duty = ((upperDuty[pin]>>8) + lowerDuty[pin] ) / 65535.0;
			int frequency = ((upperFrequency[pin]>>8) + lowerFrequency[pin] );

			if(pwm[pin]->setupPwmOut((float)frequency,duty)==0){
				Setup[pin]=CAN_PWM_SETUP_OUT;
			}
			break;
		}
		case CAN_PWM_TASK_OUT:
			if(Setup[pin]==CAN_PWM_SETUP_OUT)
				printf("%f\n",duty[pin]),
				pwm[pin]->pwmWrite(this->duty[pin]);
			Task[pin]=CAN_PWM_TASK_NULL;
			break;

		default:
			break;
		}
	}
}
