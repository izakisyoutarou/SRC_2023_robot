#pragma once
#include "ichigoplus/layer_driver/base/can.hpp"
#include "ichigoplus/layer_driver/circuit/can_id.hpp"
#include "ichigoplus/layer_driver/base/mcutime.h"
#include "robomaster_esc.hpp"
#include <vector>

namespace robomaster {

class RobomasterIO:public CanHandler {
public:
	RobomasterIO(Can &can,can::CanId node_id);
	int setup();
	void cycle();
    void print();
    void addESC(RobomasterESC &esc);

	int64_t lastReadTime();
    bool isReceived() const{return can_received;}
	int canRead(int id,int number,unsigned char data[8]);
	int canId(int id);

private:
	int64_t last_time;
    bool can_received = false;

    std::vector<RobomasterESC*> escs;

    char data6 = 0; //確認用

    const can::CanId send_id;
    const can::CanId reception_id;
};
}
