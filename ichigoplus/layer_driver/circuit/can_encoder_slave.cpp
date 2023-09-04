#include "can_encoder_slave.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"
#include "ichigoplus/layer_driver/base/mcutime.h"
using namespace rp_lib;

namespace encoder {
CanEncoderSlave::CanEncoderSlave(Can &can,int number,int dipNumber,EncoderCount &enc){
										/*  識別番号	　　ロータリーSWの値*/
	can.addHandler(this);
	this->enc = &enc;
	id=0x440+number+dipNumber*4;//dipNumber*(mainで宣言するインスタンスの数)
	Mode=CAN_ENC_SEN_NULL;
	CountMemory=0;
	Setflag=false;
	OrderFlag=false;
	frequency=65535;
	time=millis();
}


int CanEncoderSlave::setup(){
	int i=0;
	i+=canSetup();
	i+=enc->setup();
	canSetId(id-0x040);
	canSetId(id+0x001);
	return i;
}

int CanEncoderSlave::canId(int id){
		if(Mode!=CAN_ENC_SEN_ORDER)return (this->id-0x040==id);
		else return ((this->id+0x001)==id);
}

int CanEncoderSlave::canRead(int id,int number,unsigned char data[8]){

	if(number==1 && data[0]==0){
		Setflag = true;
		CountMemory=enc->count();
	}
	if(number==3 && data[0]==1 && Setflag==true){
		frequency=(unsigned short)uchar2_to_ushort(data+1);
		printf("%d\n",frequency);
		if(frequency == 65535)Mode=CAN_ENC_SEN_ORDER;
		else Mode=CAN_ENC_SEN_TRANS;
	}
	if(number==2)OrderFlag=true;
	return 0;
}

void CanEncoderSlave::cycle(){
	enc->cycle();
	if(Setflag == true){
		unsigned char data[8]={};

		if(millis()-time >= frequency && Mode==CAN_ENC_SEN_TRANS){
			time=millis();
			int_to_uchar4(data,enc->count()-CountMemory);
			canWrite(id,4,data);

		}
		if(OrderFlag==true){
			int_to_uchar4(data,enc->count()-CountMemory);
			printf("id:0x%x\n",id);
			canWrite(id,4,data);
			OrderFlag = false;
		}
	}
}
}