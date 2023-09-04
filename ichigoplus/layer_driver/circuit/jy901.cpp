#include "jy901.hpp" //ver2.1
#include "ichigoplus/lib_src/utilplus.hpp"
#include <stdio.h>

using rp_lib::f_pi;

Jy901::Jy901(Serial &serial){
	this->serial = &serial;
	this->i = 0;
	this->angXtemp = 0;
	this->angYtemp = 0;
	this->angZtemp = 0;
	this->angleXData = 0;
	this->angleYData = 0;
	this->angleZData = 0;
	this->accelXData = 0;
	this->accelYData = 0;
	this->accelZData = 0;
	this->readingData[15] = {0};
	this->readingCount = 0;
	this->readStart = false;
	this->fangleX = 0.0f;
	this->fangleY = 0.0f;
	this->fangleZ = 0.0f;
	this->sum = 0;
	this->sendData[0] = {0xFF};
	this->sendData[1] = {0xAA};
	this->sendData[2] = {0};
	this->sendData[3] = {0};
	this->sendData[4] = {0};
	this->ledMode = LED_LOW;
	this->angleXRoopCount = 0;
	this->angleYRoopCount = 0;
	this->angleZRoopCount = 0;
	this->oldAngleXData = 0;
	this->oldAngleYData = 0;
	this->oldAngleZData = 0;
	this->initialAngleXData = 0;
	this->initialAngleYData = 0;
	this->initialAngleZData = 0;
	this->initialXSetStart = 0;
	this->initialYSetStart = 0;
	this->initialZSetStart = 0;
}

int Jy901::setup(){
	serial->setup(9600,(*this));

	sendData[2] = {0x04};
	sendData[3] = {0x06};
	sendData[4] = {0x00};
	for(i = 0;i < 5;i++){
		serialWriteChar(sendData[i]);//ボーレート変更(ジャイロが初期状態のときのみ反映される)
	}

	delay_ms(20);
	serial->setup(115200,(*this));

	sixAxis();
	calibrate();
	reset();
	return 0;
}

void Jy901::serialReadChar(char data){
	readingData[readingCount] = data;
	if(readingData[0] == 0x55) readStart = true;
	if(readStart){
		if(readingCount < 10) sum += readingData[readingCount];
		readingCount++;
		if(readingCount >= 11){
			if(sum == readingData[10]){
				float dt = float(micros() - old_time) / 1000000.f;
				switch (readingData[1]){
				case 0x50: //Time
					break;
				case 0x51: //Acceleration
					/*accelXData = (float)((readingData[3]<<8) | readingData[2])/32768*16*9.81;
					accelYData = (float)((readingData[5]<<8) | readingData[4])/32768*16*9.81;
					accelZData = (float)((readingData[7]<<8) | readingData[6])/32768*16*9.81;*/
					break;
				case 0x52: //Angular Velocity
					break;
				case 0x53: //Angle

					angXtemp = (readingData[3]<<8) | (readingData[2]);
					angYtemp = (readingData[5]<<8) | (readingData[4]);
					angZtemp = (readingData[7]<<8) | (readingData[6]);

					if(angXtemp<32768) angleXData=-angXtemp;
					else angleXData=-angXtemp+65536;
					if(initialXSetStart){
						initialAngleXData=angleXData;
						initialXSetStart=0;
						oldAngleXData=angleXData;
					}else{
					if(over(angleXData)&&under(oldAngleXData)){
						angleXRoopCount--;
					}else if(over(oldAngleXData)&&under(angleXData)){
						angleXRoopCount++;
					}
					oldAngleXData=angleXData;
					fangleX=(angleXData-initialAngleXData)/32768.0f*f_pi;
					fangleX+=angleXRoopCount*2*f_pi;
					ang_vel_x = (fangleX - old_fangle_x) / dt;
					old_fangle_x = fangleX;
					}

					if(angYtemp<32768) angleYData=-angYtemp;
					else angleYData=-angYtemp+65536;
					if(initialYSetStart){
						initialAngleYData=angleYData;
						initialYSetStart=0;
						oldAngleYData=angleYData;
					}else{
					if(over(angleYData)&&under(oldAngleYData)){
						angleYRoopCount--;
					}else if(over(oldAngleYData)&&under(angleYData)){
						angleYRoopCount++;
					}
					oldAngleYData=angleYData;
					fangleY=(angleYData-initialAngleYData)/32768.0f*f_pi;
					fangleY+=angleYRoopCount*2*f_pi;
					ang_vel_y = (fangleY - old_fangle_y) / dt;
					old_fangle_y = fangleY;
					}

					if(angZtemp<32768) angleZData=-angZtemp;
					else angleZData=-angZtemp+65536;
					if(initialZSetStart){
						initialAngleZData=angleZData;
						initialZSetStart=0;
						oldAngleZData=angleZData;
					}else{
					if(over(angleZData)&&under(oldAngleZData)){
						angleZRoopCount--;
					}else if(over(oldAngleZData)&&under(angleZData)){
						angleZRoopCount++;
					}
					oldAngleZData=angleZData;
					fangleZ=(angleZData-initialAngleZData)/32768.0f*f_pi;
					fangleZ+=angleZRoopCount*2*f_pi;
					ang_vel_z = (fangleZ - old_fangle_z) / dt;
					old_fangle_z = fangleZ;
					}

					old_time = micros();

					break;
				case 0x54: //Magnetic
					break;
				case 0x55: //Port status
					break;
				case 0x56: //Atmospheric pressure and Height
					break;
				case 0x57: //Longitude and Latitude
					break;
				case 0x58: //Ground speed
					break;
				case 0x59: //Quaternion
					break;
				case 0x5A: //Satellite positioning accuracy
					break;
				}
			}
			readingCount = 0;
			readStart = false;
			sum = 0;
		}
	}
}

void Jy901::cycle(){
	angVelX(-ang_vel_x);
	angVelY(-ang_vel_y);
	angVelZ(-ang_vel_z);
	angleX(-fangleX);
	angleY(-fangleY);
	angleZ(-fangleZ);
}
