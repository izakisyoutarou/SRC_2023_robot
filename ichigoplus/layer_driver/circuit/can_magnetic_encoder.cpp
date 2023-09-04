#include "can_magnetic_encoder.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"
#include "ichigoplus/layer_driver/base/mcutime.h"
using namespace rp_lib;

namespace encoder {
CanMagneticEncoder::CanMagneticEncoder(Can &can, int number, int cpr){
	id = CAN_MAGNETIC_ENCODER_ID + number;
	cpr_ = cpr;
	can.addHandler(this);
}

int CanMagneticEncoder::setup(){//default=2ms
	return setup(2);
}


int CanMagneticEncoder::setup(uint16_t send_time){
	unsigned char data[8] = {send_time};
	if(!is_can_setuped) {
		int error = canSetup();
		error += canSetId(id);
		if(error == 0) is_can_setuped = true;
		else return error;
	}
	
	int64_t time = millis();
	is_receivable = false;
	while(millis() - time <= 1000 && !is_receivable){
		canWrite(id, 2, data);
		delay_ms(10);
	}
	return !is_receivable;
}

void CanMagneticEncoder::cycle(){
	count_ = is_reverse ? -bit : bit;
	radian_ = (float)count_ / cpr_ * 2.f * f_pi;
	degree_ = rtod(radian_);
}

int CanMagneticEncoder::canRead(int id, int number, unsigned char data[8]){
	bit = uchar4_to_int(data);
	is_receivable = true;
	return 0;
}

int CanMagneticEncoder::canId(int id){
	return this->id == id;
}
}
