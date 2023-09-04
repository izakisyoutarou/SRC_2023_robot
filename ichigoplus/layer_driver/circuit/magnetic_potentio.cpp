#include "magnetic_potentio.hpp"

namespace encoder {
MagneticPotentio::MagneticPotentio(Analog &potentio,CanMagneticEncoder &magneticEnc){
	this->potentio = &potentio;
	this->magneticEnc =&magneticEnc;
}
int MagneticPotentio::setup(int potentio_min,int potentio_max,int magnetic_enc_min,int range){
	int error = 0;
	error += potentio->setupAnalogIn();
	error += magneticEnc->setup(10);
	potentio_rev_flag = 0;
	count = 0;
	magnetic_enc_count = 0;
	potentio_count = 0;
	potentio_val_memo[POTENTIO_NUM] ={};
	potentio_value = 0;
	potentio_sum = 0;
	this->potentio_min = potentio_min;
	this->potentio_max = potentio_max;
	this->magnetic_enc_min  = magnetic_enc_min;
	step_range = range;
	return error;
}

void MagneticPotentio::rev(bool potentio_rev,bool magnetic_enc_rev){
	potentio_rev_flag = potentio_rev;
	magneticEnc->rev(magnetic_enc_rev);
}
void MagneticPotentio::cycle(){
	potentio_value = 0;
	/*if(potentio_rev_flag)potentio_value = 4096 - (int)(potentio->analogRead() * 4096);
	else*/ potentio_value = (int)(potentio->analogRead()*4096);


#if 1
	potentio_sum = 0;
	for(int i=(POTENTIO_NUM-1);i>0;i--){
		potentio_val_memo[i]=potentio_val_memo[i-1];
		potentio_sum += potentio_val_memo[i];
	}
	potentio_val_memo[0] = potentio_value;
	potentio_sum += potentio_val_memo[0];
	potentio_value =(int)( potentio_sum / (POTENTIO_NUM));
#endif
	magneticEnc->cycle();
	potentio_count = potentio_value - potentio_min;
	if(potentio_count < 0)potentio_count += 4096;
	if(potentio_rev_flag)potentio_count = 4096 - potentio_count;
	magnetic_enc_count = magneticEnc->count() - magnetic_enc_min;
	if(magnetic_enc_count < 0)magnetic_enc_count += 4096;
}
int MagneticPotentio::magPoteCount(){       //return Encoder count(0~4096*rotation_number)
	if(magnetic_enc_count  >= THRESHOLD&&potentio_count >= THRESHOLD){
		count = magnetic_enc_count - 4096;
		potentio_count = 10;
	}else if(potentio_count >= THRESHOLD){
		potentio_count = 10;
	}else if(potentio_count <= 100&&magnetic_enc_count >= THRESHOLD){
		count = magnetic_enc_count - 4096;
	}
	else{

		int step = (potentio_count / step_range);
		if(step % 2){
			if(magnetic_enc_count >= 1024)count = magnetic_enc_count + ((step / 2) * 4096);
			else count = magnetic_enc_count + (((step+1) / 2) * 4096);
		}
		else {
			if(magnetic_enc_count <= 3072)count = magnetic_enc_count + ((step / 2) * 4096);
			else if(step > 0)count = magnetic_enc_count + (((step - 1) /2) * 4096);
			else count = magnetic_enc_count;// - 4096;
		}
	}
	return count;
}
int MagneticPotentio::magneticEncCount(){ //return now magneticEncCount
	return magneticEnc->count();
}
int MagneticPotentio::potentioCount(){//return now magneticEncCount
	//int potentio_count = 0;
	//if(potentio_rev_flag)this->potentio_count = 4096 - (int)(potentio->analogRead() * 4096);
	//else this->potentio_count = (int)(potentio->analogRead()*4096);
	return /*(int)(potentio->analogRead()*4096);*/potentio_count;
}
}