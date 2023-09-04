#include "can_motor_driver.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"
using namespace rp_lib;

CanMotorDriver::CanMotorDriver(Can &can,int canMdId){
	can.addHandler(this);
	this->canMdId=canMdId+CAN_MOTOR_DUTY;
	dutyValue=0;
	runSetupOnce=false;
	turnOver=false;
	startFlag=true;
	cw=0;
	oldData[0]=0;
	oldData[1]=0;
	mdId=canMdId;
	time=millis();
	duty(0.0f);
	limitValue=0;
}

CanMotorDriver::CanMotorDriver(Can &can,int canMdId,int limitMode,float value){
	can.addHandler(this);
	this->canMdId=canMdId+CAN_MOTOR_DUTY;
	dutyValue=0;
	runSetupOnce=false;
	turnOver=false;
	startFlag=true;
	cw=0;
	oldData[0]=0;
	oldData[1]=0;
	mdId=canMdId;
	time=millis();
	duty(0.0f);
	limitValue=0;
	currentLimit(limitMode,value);
}

int CanMotorDriver::setup(){
	int i=0;
	if(!canSetup()){
		runSetupOnce=true;
	}else{
		i=1;
	}
	startFlag=true;
	return i;
}

void CanMotorDriver::cycle(){
	int works[4];
	int i=0;
	float oldDuty=0;
	unsigned char data[8];
	oldDuty=dutyValue;
	for(i=0;i<8;i++)data[i]=0;
	for(i=3;i>=0;i--){
		works[3-i]=(int)((int)oldDuty/(int)std::pow(16.f,(float)i));
		oldDuty-=(int)(((int)oldDuty/(int)std::pow(16.f,(float)i))*(int)std::pow(16.f,(float)i));
	}
	if(!cw)
		data[1]+=128;
	data[1]+=works[0]*16;
	data[1]+=works[1];
	data[0]+=works[2]*16;
	data[0]+=works[3];
	if(canMdId!=CAN_MOTOR_FREE+mdId){
		//if(dutyValue!=outDuty){
			if(oldData[0]!=data[0]||oldData[1]!=data[1]){
			canWrite(canMdId,2,data);
			oldData[0]=data[0];
			oldData[1]=data[1];
			time=millis();
		}else if(millis()-time>20){
			canWrite(canMdId,2,data);
			time=millis();
		}
		if(startFlag==true){
			canWrite(canMdId,2,data);
			startFlag=false;
		}
	}else{
		startFlag=true;
		canWrite(canMdId,2,data);
	}
	outDuty=dutyValue;
	if(turnOver)outDuty*=-1;
	if(!cw)outDuty*=-1;
}

void CanMotorDriver::outRev(bool value){
	turnOver=value;
}

void CanMotorDriver::duty(float value){
	canMdId=mdId+CAN_MOTOR_DUTY;
	//outDuty=value;
	value=constrain(value,-1.0f,1.0f);
	if(value>=0)
		cw=!turnOver;
	else{
		cw=turnOver;
		value*=(-1);
	}
	dutyValue=value*resolution();
}

float CanMotorDriver::currentLimit(int limitMode,float value){
	unsigned char data[8];
	limitValue = floatlimit(0.0,value,165.0);
	if(limitMode == LIMIT_PEAK){
		canMdId = mdId + CAN_MOTOR_CURRENT_LIMIT;
		data[0] = 0x01;
	}else if(limitMode == LIMIT_AVG){
		canMdId = mdId + CAN_MOTOR_CURRENT_LIMIT;
		data[0] = 0x02;
	}
    float_to_uchar4(data+1,value);
	canWrite(canMdId, 5, data);
	return limitValue;
}
