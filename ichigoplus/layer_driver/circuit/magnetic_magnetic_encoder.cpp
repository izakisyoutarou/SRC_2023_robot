#include "magnetic_magnetic_encoder.hpp"

namespace encoder {
MagneticMagneticEncoder::MagneticMagneticEncoder(CanMagneticEncoder &singleTurnEnc,CanMagneticEncoder &multiTurnEnc){
	this->singleTurnEnc=&singleTurnEnc;
	this->multiTurnEnc =&multiTurnEnc;
}
int MagneticMagneticEncoder::setup(int single_enc_min,int single_enc_max,int multi_enc_min,int range){
	int error = 0;
	error += singleTurnEnc->setup();
	error += multiTurnEnc->setup();
	count = 0;
	multi_turn_enc_count = 0;
	single_turn_enc_count = 0;
	this->single_enc_min = single_enc_min;
	this->single_enc_max = single_enc_max;
	this->multi_enc_min  = multi_enc_min;
	step_range = range;
	return error;
}
void MagneticMagneticEncoder::rev(bool single_enc_rev,bool multi_enc_rev){
	singleTurnEnc->rev(single_enc_rev);
	multiTurnEnc->rev(multi_enc_rev);
}
void MagneticMagneticEncoder::cycle(){
	singleTurnEnc->cycle();
	multiTurnEnc->cycle();
	single_turn_enc_count = singleTurnEnc->count() - single_enc_min;
	if(single_turn_enc_count < 0)single_turn_enc_count += 4096;
	multi_turn_enc_count = multiTurnEnc->count() - multi_enc_min;
	if(multi_turn_enc_count < 0)multi_turn_enc_count += 4096;
}



int MagneticMagneticEncoder::magMagCount(){       //return Encoder count(0~4096*rotation_number)
	int step = (single_turn_enc_count / step_range);
	if(step % 2){
		if(multiTurnEnc->count() >= 1024)count = multi_turn_enc_count + (step * 2048);
		else count = multi_turn_enc_count + ((step + 1) * 2048);
	}
	else {
		if(multiTurnEnc->count() <= 3072)count = multi_turn_enc_count + (step * 2048);
		else if(step > 0)count = multi_turn_enc_count + ((step - 1) * 2048);
		else count = multi_turn_enc_count - 4096;
	}

	if(single_turn_enc_count  >= THRESHOLD&&multi_turn_enc_count >= THRESHOLD){
		count = multi_turn_enc_count - 4096;
		single_turn_enc_count = 10;
	}else if(single_turn_enc_count >= THRESHOLD){
		single_turn_enc_count = 10;
	}else if(multi_turn_enc_count >= THRESHOLD){
		count = multi_turn_enc_count - 4096;
	}
	else{

		step = (single_turn_enc_count / step_range);
		if(step % 2){
			if(multi_turn_enc_count >= 1024)count = multi_turn_enc_count + ((step / 2) * 4096);
			else count = multi_turn_enc_count + (((step+1) / 2) * 4096);
		}
		else {
			if(multi_turn_enc_count <= 3072)count = multi_turn_enc_count + ((step / 2) * 4096);
			else if(step > 0)count = multi_turn_enc_count + (((step - 1) /2) * 4096);
			else count = multi_turn_enc_count;// - 4096;
		}
	}

	return count;
}

int MagneticMagneticEncoder::multiTurnEncCount(){ //return now multiTurnEncCount
	return multiTurnEnc->count();
}

int MagneticMagneticEncoder::singleTurnEncCount(){//return now multiTurnEncCount
	return singleTurnEnc->count();
}
}
