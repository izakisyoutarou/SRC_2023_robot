#pragma once

#include "ichigoplus/layer_driver/base/can.hpp"
#include "ichigoplus/layer_driver/base/encoder.hpp"

namespace encoder {
class CanEncoderSlave:public CanHandler{
private:
	EncoderCount *enc;
	int number;
	int id;
	int Mode;
	int CountMemory;
	bool Setflag,OrderFlag;
	int encValue;
	unsigned short frequency;
	long lastGetTimeData;
	int64_t time;
	enum{
		CAN_ENC_SEN_NULL=0,
		CAN_ENC_SEN_TRANS,
		CAN_ENC_SEN_ORDER,
	};
public:
	CanEncoderSlave(Can &can,int number,int dipNumber,EncoderCount &enc);
	int setup();
	void cycle();
	int count(){return enc->count();}
	long lastReadTime();
	int canRead(int id,int number,unsigned char data[8]);
	int canId(int id);
};
}
