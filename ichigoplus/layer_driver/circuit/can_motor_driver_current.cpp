#include "can_motor_driver_current.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"
using namespace rp_lib;

CanMdCurrent::CanMdCurrent(Can &can,int number){
	can.addHandler(this);
	boardNumber = CAN_MD_CURRENT_ID + number;
	value = 0.0;
}

int CanMdCurrent::setup(){
	int i = 0;
	i += canSetup();
	canSetId(boardNumber);
	return i;
}

int CanMdCurrent::canRead(int id,int number,unsigned char data[8]){
	value = uchar4_to_float(data);
}

int CanMdCurrent::canId(int id){
	return (id == boardNumber);
}

float CanMdCurrent::readCurrent(){
	return value;
}
