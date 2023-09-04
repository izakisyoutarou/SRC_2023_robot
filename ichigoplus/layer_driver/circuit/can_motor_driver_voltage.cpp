#include "can_motor_driver_voltage.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"
using namespace rp_lib;

CanMdVoltage::CanMdVoltage(Can &can,int number){
	can.addHandler(this);
	boardNumber = CAN_MD_VOLTAGE_ID + number;
	value = 0.0;
}

int CanMdVoltage::setup(){
	int i = 0;
	i += canSetup();
	canSetId(boardNumber);
	return i;
}

int CanMdVoltage::canRead(int id,int number,unsigned char data[8]){
	value = uchar4_to_float(data);
}

int CanMdVoltage::canId(int id){
	return (id == boardNumber);
}

float CanMdVoltage::read(){
	return value;
}
