#pragma once

/*******************************************
 * can_encoder ver1.0 2022/2/3
 * Classes of can encoder node for ichigo system.
 *
 * [Dependency]
 * canplus
 * encoderplus
 * mcutime
 *
 * [Note]
 *
 * [Change history]
 * ver1.0 2022/ 2/ 3 Support new Encoder.
 * ver0.1 2015/ 3/ 5 The first version.Test.
 ******************************************/

#include "ichigoplus/layer_driver/base/can.hpp"
#include "ichigoplus/layer_driver/base/encoder.hpp"
#include "ichigoplus/layer_driver/base/mcutime.h"

#define CAN_ENCODER_ID 0x440

namespace encoder {
class CanEncoder:public CanHandler,public EncoderCount {
private:
	int number;
	int encValue;
	unsigned short frequency;
	int64_t lastGetTimeData;
	int id;
	int cpr_ = 1;
	bool is_reverse = false;
	int count_ = 0;
	float radian_ = 0.f;
public:
	CanEncoder(Can &can,int number);
	CanEncoder(Can &can,int number, unsigned short frequencyMillisArg);
	int setup() override;
	void cycle() override;
	void rev(bool rev) override { is_reverse = rev; }
	float radian() const override { return radian_; }
	void cpr(int cpr) override { cpr_ = cpr; }
	int count() const override { return count_; }
	int64_t lastReadTime();
	int canRead(int id,int number,unsigned char data[8]);
	int canId(int id);
};
}
