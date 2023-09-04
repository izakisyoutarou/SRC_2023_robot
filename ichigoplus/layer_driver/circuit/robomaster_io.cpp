#include "robomaster_io.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"

#define CAN_INITIALIZATION_ID 0x000
#define CAN_SETUP_ID 0x001
#define CAN_FIRST_CURRENT_ID 0x002
#define CAN_SECOND_CURRENT_ID 0x003

using namespace rp_lib;
using namespace can;
using namespace can::can_id::robomaster_io;

namespace robomaster{
RobomasterIO::RobomasterIO(Can &can,CanId node_id) : send_id(calcSendId(node_id)), reception_id(calcReceptionId(node_id)){
	last_time = millis();
	can.addHandler(this);
    escs.reserve(8);

    //IO側のESC情報の初期化
    const CanId can_initialization_id = send_id | CAN_INITIALIZATION_ID;
    unsigned char data[8];
    canWrite(can_initialization_id,1,data);
}

int RobomasterIO::setup(){
	if(canSetup()) return 1;

	return 0;
}

void RobomasterIO::cycle(){
    const CanId can_first_id = send_id | CAN_FIRST_CURRENT_ID;
    const CanId can_second_id = send_id | CAN_SECOND_CURRENT_ID;

    unsigned char data_first[8] = {0,0,0,0,0,0,0,0};
    unsigned char data_second[8] = {0,0,0,0,0,0,0,0};

    for(const auto &esc : escs){
        const int current = esc->current();
        const int motor_num = esc->config().number;
        unsigned char data0 = (unsigned char)(current >> 8);
        unsigned char data1 = (unsigned char)(current & 0xFF);

        if(motor_num<4) {
            data_first[motor_num*2] = data0;
            data_first[motor_num*2+1] = data1;
        }
        else{
            data_second[(motor_num-4)*2] = data0;
            data_second[(motor_num-4)*2+1] = data1;
        }
    }

    canWrite(can_first_id,8,data_first);
    canWrite(can_second_id,8,data_second);
}

void RobomasterIO::addESC(RobomasterESC &esc){
    const CanId can_send_id = send_id | CAN_SETUP_ID;
    const CanId can_reception_id = reception_id | esc.config().number;

    unsigned char data[8];

    ushort_to_uchar2(data, (unsigned short)esc.config().number);
    ushort_to_uchar2(data+2, (unsigned short)esc.config().send_period_ms);
    ushort_to_uchar2(data+4, (unsigned short)esc.config().count_resolution);

    data[6] = esc.config().send_count<<7 | esc.config().send_rpm<<6 | esc.config().send_torque<<5;
    data6 = data[6];

    canWrite(can_send_id,7,data);

    canSetId(can_reception_id);

    escs.push_back(&esc);
}

void RobomasterIO::print(){
    printf("can received  %d\n",can_received);
    printf("esc numbers  %d\n",escs.size());
    printf("send param(data6)  %d\n",data6);
    printf("receive ids : ");
    for(const auto& esc : escs) printf("%x ,",reception_id | esc->config().number);
    printf("\n");
}

int64_t RobomasterIO::lastReadTime(){
	return last_time;
}

int RobomasterIO::canRead(int id,int number,unsigned char data[8]){
    const int esc_number = id & 0xF;
    const int count = uchar4_to_int(data);
    const int rpm = uchar2_to_short(data+4);
    const int torque = uchar2_to_short(data+6);

    for(auto &esc : escs){
        const int esc_num = esc->config().number;
        if(id == (reception_id | esc_num)){
            esc->setData(count, rpm, torque);
        }
    }

    can_received = true;
	last_time = millis();
	return 0;
}

int RobomasterIO::canId(int id){
    for(const auto& esc : escs){
        if(id==(reception_id | esc->config().number)) return true;
    }
    return false;
}

}
