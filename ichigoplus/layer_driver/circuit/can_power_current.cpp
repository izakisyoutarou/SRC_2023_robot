#include "can_power_current.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"
using namespace rp_lib;

CanPowerCurrent::CanPowerCurrent(Can &can,int number){
	can.addHandler(this);
	boardNumber = CAN_POWER_CURRENT_ID + number;
	value = 0.0;
}

int CanPowerCurrent::setup(){
	int i = 0;
	i += canSetup();
	canSetId(boardNumber);
	return i;
}

int CanPowerCurrent::canRead(int id,int number,unsigned char data[8]){
	value = uchar4_to_float(data);
}

int CanPowerCurrent::canId(int id){
	return (id == boardNumber);
}

float CanPowerCurrent::readCurrent(){
	return value;
}
