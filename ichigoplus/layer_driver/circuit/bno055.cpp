#include "bno055.hpp"
#include "ichigoplus/layer_driver/base/mcutime.h"
#include "ichigoplus/lib_src/utilplus.hpp"

using rp_lib::f_pi;

Bno055::Bno055(Serial &serial){
	serialHandlerSetup(&serial);
    time_read_cycle = 0;
    read_count = 0;
    roop_count = 0;
    setup_done = false;
	imu_rad_yaw = 0.f;
	imu_rad_roll = 0.f;
	imu_rad_pitch = 0.f;
	imu_rad_yaw_old = 0.f;
	imu_rad_roll_old = 0.f;
	imu_rad_pitch_old = 0.f;
	time_vel = 0;

    for(int i=0; i<17; i++)
    	read_baff[i] = 0;

    for(int i=0; i<8; i++)
    	send_data[i] = 0;
}


int Bno055::setup(){
	serial->setup(115200,(*this));

	delay_ms(650);
	checkChipId();
	delay_ms(19);//data sheet P21
	setOpeMode(OPERATION_MODE_CONFIG);
	delay_ms(19);
	resetChip();
	delay_ms(650);
	checkChipId();
	delay_ms(19);
	setPowerMode(POWER_MODE_NORMAL);
	delay_ms(19);
	setExternalClk();
	delay_ms(19);
	setOpeMode(OPERATION_MODE_IMUPLUS);
	delay_ms(400);

	char error_count = 0;
	/*正しくセットアップできた時の文字列*/
	char correct_baff[17] = {0xEE,0x0A,0xBB,0x01,0xA0,0xEE,0x01,0xEE,0xBB,0x01,0xA0,0xEE,0x01,0xEE,0x01,0xEE,0x01};

	/*受信した文字列がただしいかの確認*/
	for(int i=0; i<17; i++){
		if(read_baff[i] != correct_baff[i])error_count++;
	}

    for(int i=0; i<17; i++)
    	read_baff[i] = 0;
	read_count = 0;
	setup_done = true;
	return error_count;
}

void Bno055::cycle(){
	if(setup_done)
		imuDataRead();
}


bool Bno055::checkChipId(){
/*IDチェック*/
	readData(BNO055_CHIP_ID_ADDR,1);
	return 1;//return0にならなかった時点でエラーですよ
}

bool Bno055::resetChip(){
	/*チップをリセット*/
	send_data[0] = 0x20;
	writeData(BNO055_SYS_TRIGGER_ADDR,1,send_data);
}

bool Bno055::setOpeMode(bno055_opmode_t mode){
	/*指定されたモードに変更*/
	send_data[0] = mode;
	writeData(BNO055_OPR_MODE_ADDR,1,send_data);
	return 1;
}

bool Bno055::setPowerMode(bno055_powermode_t mode){
	/*パワーモードの変更*/
	send_data[0] = mode;
	writeData(BNO055_PWR_MODE_ADDR,1,send_data);
	return 1;
}

bool Bno055::setPageMode(char page){
	/*ページの変更*/
	send_data[0] = page;
	writeData(BNO055_PAGE_ID_ADDR,1,send_data);
	return 0;
}

bool Bno055::setExternalClk(){
	/*外部クロックの設定*/
	send_data[0] = 0x80;//外部クロックを有効にするcommand
	writeData(BNO055_SYS_TRIGGER_ADDR,1,send_data);
	return 0;
}

void Bno055::imuDataRead(){
	if(millis() - time_read_cycle > 10){

		if(read_count == 8){
			//time_read_cycle
			int rad_baff = 0;//ここで2バイトのデータにしてそれを計算してimu_radへ

			//yaw//データ範囲は0-360ぽい
			//read_baff[3] = 0x0;
			rad_baff = read_baff[2];
			rad_baff |= read_baff[3] << 8;

			imu_rad_yaw = (float)rad_baff * 0.001090831;//データ範囲は0-360ぽい

			//roll +-90degらしい
			rad_baff = read_baff[4];
			rad_baff |= read_baff[5] << 8;
			if(rad_baff > 60000)rad_baff -= 65535;
			imu_rad_roll = (float)rad_baff * 0.001090831;

			//pitch　±180
			rad_baff = read_baff[6];
			rad_baff |= read_baff[7] << 8;
			if(rad_baff > 60000)rad_baff -= 65535;
			imu_rad_pitch = (float)rad_baff * 0.001090831;

			if(imu_rad_yaw >(3*f_pi / 2.f)&& imu_rad_yaw_old < f_pi / 2.f)
				roop_count--;
			else if(imu_rad_yaw_old >(3*f_pi / 2.f)&& imu_rad_yaw < f_pi / 2.f)
				roop_count++;

			angleZ(imu_rad_yaw + (roop_count * f_pi * 2));
			angleX(imu_rad_roll);
			angleY(imu_rad_pitch);

			float vel_checker = 0.f;
			vel_checker = (imu_rad_yaw - imu_rad_yaw_old) / float(micros() - time_vel) * 1000000;
			if(vel_checker < -34.9)angVelZ(-34.9f);
			else if(vel_checker > 34.9)angVelZ(34.9f);
			else angVelZ(vel_checker);

			vel_checker = (imu_rad_roll - imu_rad_roll_old) / float(micros() - time_vel) * 1000000;
			if(vel_checker < -34.9)angVelX(-34.9f);
			else if(vel_checker > 34.9)angVelX(34.9f);
			else angVelX(vel_checker);

			vel_checker = (imu_rad_pitch - imu_rad_pitch_old) / float(micros() - time_vel) * 1000000;
			if(vel_checker < -34.9)angVelY(-34.9f);
			else if(vel_checker > 34.9)angVelY(34.9f);
			else angVelY(vel_checker);

			time_vel = micros();
			imu_rad_yaw_old = imu_rad_yaw;
			imu_rad_roll_old = imu_rad_roll;
			imu_rad_pitch_old = imu_rad_pitch;

			//受信準備。今までのデータを消す。
			for(int i=0; i<8; i++)
				read_baff[i] = 0;
			read_count = 0;
		}
		else if(read_count > 8){
			for(int i=0; i<8; i++)
				read_baff[i] = 0;
			read_count = 0;
		}


		readData(BNO055_EULER_H_LSB_ADDR,6);
		time_read_cycle = millis();
	}

}


void Bno055::serialReadChar(char value){
	/*シリアル受信するたびに呼び出される*/
	read_baff[read_count] = value;
	read_count++;
}


void Bno055::readData(bno055_reg_t send_reg,int length){
	/*受信用関数*/
	serial->writeChar(0xAA);
	serial->writeChar(0x01);;
	serial->writeChar(send_reg);;
	serial->writeChar(length);
}

void Bno055::writeData(bno055_reg_t send_reg,char length,char data[8]){
	/*書き込み、モード変更用関数*/
	serial->writeChar(0xAA);
	serial->writeChar(0x00);
	serial->writeChar(send_reg);
	serial->writeChar(length);
	for(int i=0; i < length; i++){//ここ間違えてそう
		serial->writeChar(data[i]);
	}
}



