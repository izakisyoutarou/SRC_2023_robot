#include "can_encoder.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"
#include "ichigoplus/layer_driver/base/mcutime.h"
#include <string.h>
#include <stdio.h>
using namespace rp_lib;

namespace encoder {
CanEncoder::CanEncoder(Can &can,int number){
	id=CAN_ENCODER_ID+number;
	lastGetTimeData=millis();
	frequency=65535;
	can.addHandler(this);
}

CanEncoder::CanEncoder(Can &can,int number,unsigned short frequencyArg){
	id=CAN_ENCODER_ID+number;
	lastGetTimeData=millis();
	frequency=frequencyArg;
	can.addHandler(this);
}

int CanEncoder::setup(){
	if(canSetup()) return 1;
	unsigned char data[8];
	canSetId(id);
	data[0]=0;
	canWrite(id-0x040,1,data);
	data[0]=1;
	ushort_to_uchar2(data+1,frequency);
	canWrite(id-0x040,3,data);
	encValue=0;
	lastGetTimeData=millis();
	return 0;
}

void CanEncoder::cycle(){
	if(frequency==65535){
		unsigned char data[8];
		long lastTime=lastGetTimeData;
		canWrite(id,0,data);
		while(lastTime==lastGetTimeData);
	}
	count_ = is_reverse ? -encValue : encValue;
	radian_ = (float)count_ / cpr_ * 2.f * f_pi;
}

int64_t CanEncoder::lastReadTime(){
	return lastGetTimeData;
}

int CanEncoder::canRead(int id,int number,unsigned char data[8]){
	encValue=uchar4_to_int(data);
	lastGetTimeData=millis();
	return 0;
}

int CanEncoder::canId(int id){
	return id==this->id;
}
}