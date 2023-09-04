#pragma once

#include "can_magnetic_encoder.hpp"

#define SINGLE_ENC_NUM 10
#define THRESHOLD 3800//???v?C??

namespace encoder {
class MagneticMagneticEncoder{
public:
	MagneticMagneticEncoder(CanMagneticEncoder &singleTurnEnc,CanMagneticEncoder &multiTurnEnc);
	int setup(int single_enc_min,int single_enc_max,int multi_enc_min,int range);
	void rev(bool single_enc_rev,bool multi_enc_rev);
	void cycle();
	int magMagCount();     //return Encoder count(0~4096*rotation_number)
	int multiTurnEncCount(); //return now multiTurnEncCount
	int singleTurnEncCount();//return now multiTurnEncCount


	CanMagneticEncoder *singleTurnEnc;
	CanMagneticEncoder *multiTurnEnc;
private:
	int flag_over_range_threshold;
	int count;
	int multi_turn_enc_count;
	int single_turn_enc_count;
	int single_enc_min;
	int single_enc_max;
	int multi_enc_min;
	int step_range;
};
}
