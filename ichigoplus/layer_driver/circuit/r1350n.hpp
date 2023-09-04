#pragma once
/*******************************************
 * can_encoderplus ver3.1 2020/11/12
 * Classes of can encoder node for ichigo system.
 *
 * [Dependency]
 * serial
 * util
 * math
 *
 * [Note]
 *
 * [Change history]
 * ver3.1 2020/11/12 fix include path, move inclusion to cpp
 * ver3.0 2020/04/02 Inheritance GyroSensor, Refactoring, by Yuta Uehara.
 * ver1.2 2019/10/24 improve rev function
 * ver1.1 2019/10/ 8 add reverse function for angleZ
 * ver1.0 2016/ 5/ 9 Some bug fix.
 * ver0.1 2015/ 3/ 5 The first version.Test.
 ******************************************/
#include "ichigoplus/layer_driver/base/serial.hpp"
#include "ichigoplus/layer_driver/base/gyro_sensor.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"

class R1350n:public SerialHandler,public GyroSensor{
public:
	R1350n(Serial &serial){this->serial=&serial;}
	void serialReadChar(char data);

	int setup() override {return serial->setup(115200,(*this));};
	void cycle() override;
	float rate(){return rateData*rp_lib::f_pi/18000.0;};
	float accelx(){return accelXData;};
	float accely(){return accelYData;};
	float accelz(){return accelZData;};
	int64_t time(){return timeData;};
	int number(){return numberData;};
	void reset(){initialAngleData=angleData;angleRoopCount=0;initialSetStart=true;numberData=0;};

private:
	enum class ReadStart{
		ON,
		OFF
	} readStart = ReadStart::OFF;
	bool over(int value){return value>9000;};
	bool under(int value){return value<-9000;};
	int angleData = 0;
	int rateData = 0;
	int accelXData = 0;
	int accelYData = 0;
	int accelZData = 0;
	int initialAngleData = 0;
	char readingData[15];
	int readingCount = 0;
	int angleRoopCount = 0;
	bool initialSetStart = true;
	float fangle = 0;
	float oldAngle = 0;
	float angleVelZ = 0;
	int oldAngleData = 0;
	int64_t timeData = 0;
	int numberData = 0;
};
