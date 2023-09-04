#include "amt_absolute_encoder.hpp"
#include "ichigoplus/layer_driver/base/mcutime.h"
#include "ichigoplus/lib_src/utilplus.hpp"

#define COUNT_RESOLUTION_12BIT 4096
#define COUNT_RESOLUTION_14BIT 16384

using namespace rp_lib;
using namespace can;
using namespace can::can_id::amt_absolute_encoder;

namespace encoder{

AMTAbsoluteEncoder::AMTAbsoluteEncoder(Serial &serial, Resolution resolution):can_id(0x00), resolution(resolution){
    run_mode = RunMode::serial;
	serialHandlerSetup(&serial);
}
AMTAbsoluteEncoder::AMTAbsoluteEncoder(Can &can, CanId node_id, Resolution resolution):can_id(calcId(node_id)), resolution(resolution){
    run_mode = RunMode::can;
    can.addHandler(this);
}

int AMTAbsoluteEncoder::setup(){
    int error = 0;
    if(run_mode==RunMode::serial){
	    error += serialSetup(this->baudrate) != 0;
    }
    else if(run_mode==RunMode::can){
        error += canSetup() != 0;
        canSetId(can_id);
        //CANエンコーダセットアップ
        // unsigned char data[8];
        // data[0] = static_cast<unsigned char>(resolution);
        // canWrite(can_id, 1, data);
    }
    setCountResolution(resolution);

	return error;
}

void AMTAbsoluteEncoder::cycle(){
    if(!cycleOnceChecker()) return;

    if(run_mode==RunMode::serial){
        serialWriteChar(address);
        is_receiving_first_data = true;
    }
    const int count = (int)count_;
    radian_ =  (is_reverse ? -1 : 1) * (float)count / count_resolution * (2.f*f_pi);
}

void AMTAbsoluteEncoder::serialReadChar(char data){
    if(is_receiving_first_data){
        observed_count = data;
        is_receiving_first_data = false;
    }
    else{
        observed_count |= data<<8;

        //チェックサム
        bool binaryArray[16];
        for(int i = 0; i < 16; i++) binaryArray[i] = (0x01) & (observed_count >> (i));

        if ((binaryArray[15] == !(binaryArray[13] ^ binaryArray[11] ^ binaryArray[9] ^ binaryArray[7] ^ binaryArray[5] ^ binaryArray[3] ^ binaryArray[1]))
          && (binaryArray[14] == !(binaryArray[12] ^ binaryArray[10] ^ binaryArray[8] ^ binaryArray[6] ^ binaryArray[4] ^ binaryArray[2] ^ binaryArray[0]))){

            count_ = observed_count;
            count_ &= 0x3FFF;

            //12bit解像度のエンコーダは位置をシフトする
            if (resolution==Resolution::bit12){
                count_ >> 2;
            }
          }
        is_receiving_first_data = true;
    }
    is_received = true;
}

int AMTAbsoluteEncoder::canRead(int id,int number,unsigned char data[8]){
    if(id==can_id){
        count_ = uchar2_to_ushort(data);
    }
    is_received = true;
}

void AMTAbsoluteEncoder::setCountResolution(Resolution resolution){
    this->resolution = resolution;
    if(resolution==Resolution::bit12) count_resolution = COUNT_RESOLUTION_12BIT;
    else if(resolution==Resolution::bit14) count_resolution = COUNT_RESOLUTION_14BIT;
}

void AMTAbsoluteEncoder::print(){
    printf("received : %d\n", is_received);
    printf("count : %d\n", (int)count_);
    printf("\n");
}
}
