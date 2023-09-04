#include "can_power_voltage.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"
using namespace rp_lib;

CanPowerVoltage::CanPowerVoltage(Can &can,int number){
	can.addHandler(this);
	boardNumber = CAN_POWER_VOLTAGE_ID + number;
	value = 0.0;
}

int CanPowerVoltage::setup(){
	int i = 0;
	i += canSetup();
	canSetId(boardNumber);
	return i;
}

int CanPowerVoltage::canRead(int id,int number,unsigned char data[8]){
	value = uchar4_to_float(data);
}

int CanPowerVoltage::canId(int id){
	return (id == boardNumber);
}

float CanPowerVoltage::readVoltage(){
	return value;
}
