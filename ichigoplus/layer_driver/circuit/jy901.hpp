#pragma once
/*******************************************
 * Jy901 2020/03/17 ver2.1
 * gyro sensor 
 *
 * [Dependency]
 * serial
 * util
 * math
 *
 * [Note]
 *
 * [Change history]
 * ver2.1 2020/03/17 Add function which can calc angular velocity from angle
 * ver2.0 2020/03/17 AngleSensor change to GyroSensor. Add reset to setup
 * ver1.0 20??/??/?? First version.
 ******************************************/
#include "ichigoplus/layer_driver/base/serial.hpp"
#include "ichigoplus/layer_driver/base/mcutime.h"
#include "ichigoplus/layer_driver/base/gyro_sensor.hpp"

class Jy901:public SerialHandler,public GyroSensor{
private:
	int i;
	int angXtemp;
	int angYtemp;
	int angZtemp;
	int angleXData;
	int angleYData;
	int angleZData;
	int initialAngleXData;
	int initialAngleYData;
	int initialAngleZData;
	int accelXData;
	int accelYData;
	int accelZData;
	char readingData[15];
	int readingCount;
	bool readStart;
	float fangleX;
	float fangleY;
	float fangleZ;
	char sum;
	char sendData[5];
	bool ledMode;
	int angleXRoopCount;
	int angleYRoopCount;
	int angleZRoopCount;
	int initialXSetStart;
	int initialYSetStart;
	int initialZSetStart;
	int oldAngleXData;
	int oldAngleYData;
	int oldAngleZData;

	float old_fangle_x = 0.f;
	float old_fangle_y = 0.f;
	float old_fangle_z = 0.f;
	float ang_vel_x = 0.f;
	float ang_vel_y = 0.f;
	float ang_vel_z = 0.f;
	int64_t old_time = 0;

	enum{
		LED_LOW,
		LED_HIGH,
	};
	int sixAxis(){
		sendData[2] = {0x24};
		sendData[3] = {0x01};
		sendData[4] = {0x00};
		for(i = 0;i < 5;i++){
			serialWriteChar(sendData[i]);//6軸モードに設定
		}
		delay_ms(20);
		return 0;
	};
	int calibrate(){
		sendData[2] = {0x02};
		sendData[3] = {0xFF};
		sendData[4] = {0x03};
		for(i = 0;i < 5;i++){
			serialWriteChar(sendData[i]);//受信するデータの決定
		}
		delay_ms(20);
		return 0;
	};
	void ledOn(){
		sendData[2] = {0x1b};
		sendData[3] = {0x00};
		sendData[4] = {0x00};
		for(i = 0;i < 5;i++){
			serialWriteChar(sendData[i]);
		}
		ledMode = LED_HIGH;
		delay_ms(20);
	};
	void ledOff(){
		sendData[2] = {0x1b};
		sendData[3] = {0x01};
		sendData[4] = {0x00};
		for(i = 0;i < 5;i++){
			serialWriteChar(sendData[i]);
		}
		ledMode = LED_LOW;
	};
	bool over(int value){return value>9000;};
	bool under(int value){return value<-9000;};
public:
	Jy901(Serial &serial);
	int setup() override;
	void serialReadChar(char data);
	void cycle() override;
	float angX(){return -fangleX;};
	float angY(){return -fangleY;};
	float angZ(){return -fangleZ;};
	int accelx(){return accelXData;};
	int accely(){return accelYData;};
	int accelz(){return accelZData;};
	void ledToggle(bool value){
		if(value) ledOn();
		else ledOff();
	};//LEDの操作をするぞ(デフォルトの状態だとtoggleする)

	void softwareReset(){
		sendData[2] = {0x00};
		sendData[3] = {0x01};
		sendData[4] = {0x00};
		for(i = 0;i < 5;i++){
			serialWriteChar(sendData[i]);//すべてを初期状態にreset(基本使うな)
		}
		delay_ms(20);
	};

	void reset(){
		initialAngleXData=angleXData;
		angleXRoopCount=0;
		initialXSetStart=1;

		initialAngleYData=angleYData;
		angleYRoopCount=0;
		initialYSetStart=1;

		initialAngleZData=angleZData;
		angleZRoopCount=0;
		initialZSetStart=1;
	};//angleX,Y,Zのデータを0にするぞ
};
