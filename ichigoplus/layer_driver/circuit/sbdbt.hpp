#pragma once

#include "ichigoplus/layer_driver/base/serial.hpp"
#include "ichigoplus/layer_driver/base/mcutime.h"
#include "dual_shock.hpp"

#define BAUDRATE			2400
//電文フォーマットのbyte数
#define ORDER_NUM			28
//ボタン数
#define DIGITAL_BUTTON_NUM	17
//電文フォーマとの開始データ
#define ORDER_START			0x80

//--- フォーマット上での対応byteの順序
#define DIGITAL_BUTTON_H	1
#define DIGITAL_BUTTON_L	2
#define ANALOG_STICK_LX		3
#define ANALOG_STICK_LY		4
#define ANALOG_STICK_RX		5
#define ANALOG_STICK_RY		6
#define ANALOG_UP			7
#define ANALOG_DOWN			8
#define ANALOG_RIGHT		9
#define ANALOG_LEFT			10
#define ANALOG_TRIANGLE		11
#define ANALOG_CIRCLE		13
#define ANALOG_CROSS		12
#define ANALOG_RECTANGLE	14
#define ANALOG_L1			15
#define ANALOG_L2			16
#define ANALOG_R1			17
#define ANALOG_R2			18
#define ANALOG_ACCEL_X_H	19
#define ANALOG_ACCEL_X_L	20
#define ANALOG_ACCEL_Y_H	21
#define ANALOG_ACCEL_Y_L	22
#define ANALOG_ACCEL_Z_H	23
#define ANALOG_ACCEL_Z_L	24
#define BATTERY_STATE		25
#define CONNECTION_STATE	26

//--- digital_buttonのbyte内での対応bit
#define	DIGITAL_UP			0x0001
#define	DIGITAL_DOWN		0x0002
#define	DIGITAL_RIGHT		0x0004
#define	DIGITAL_LEFT		0x0008

#define	DIGITAL_TRIANGLE	0x0010
#define	DIGITAL_CROSS		0x0020
#define	DIGITAL_CIRCLE		0x0040
#define	DIGITAL_RECTANGLE	0x0080

#define	DIGITAL_L1			0x0100
#define	DIGITAL_L2			0x0200
#define	DIGITAL_R1			0x0400
#define	DIGITAL_R2			0x0800

#define	DIGITAL_L3			0x1000
#define	DIGITAL_R3			0x2000
#define	DIGITAL_START		0x4000
#define	DIGITAL_SELECT		0x8000
#define	DIGITAL_PS			0xf000

//--- アナログスティックの定数
#define ANALOG_STICK_OFFSET	64
#define OFFSET_ANALOG_STICK	0x80
#define MIN_ANALOG_STICK	0
#define MAX_ANALOG_STICK	0x7f

//--- ボタン感圧
#define MAX_ANALOG_BUTTON	0xff
#define MIN_ANALOG_BUTTON	0

//--- コントローラの加速度定数
#define MIN_ACCEL_X			400
#define MAX_ACCEL_X			620
#define MID_ACCEL_X			510

#define MIN_ACCEL_Y			400
#define MAX_ACCEL_Y			620
#define MID_ACCEL_Y			500

//--- コントローラステータス
#define MAX_CONNECTION		0xff
#define ERROR_CONNECTION	0xb0
#define MAX_BATTERY			0x05
#define ERROR_BATTERY		0x02

//---　配列におけるボタン添え字
#define UP					0
#define DOWN				1
#define	RIGHT				2
#define LEFT				3
#define TRIANGLE			4
#define CROSS				5
#define CIRCLE				6
#define RECTANGLE			7
#define L1					8
#define L2					9
#define L3					10
#define R1					11
#define R2					12
#define R3					13
#define START				14
#define SELECT				15
#define PS					16

#define NAME				"dualshock3"

#define TIME_OUT			5000

typedef struct{
	float button[DIGITAL_BUTTON_NUM];
	float up, down, right, left;
	float triangle, cross, circle, rectangle;
	float l1, l2, l3, r1, r2, r3;
	float start, select, ps;
}BUTTON;

typedef struct{
	float lx, ly;
	float rx, ry;
}STICK;

typedef struct{
	float x, y, z;
}GYRO;

typedef struct{
	int connection;
	int battery;
}INFO;

typedef struct{
	BUTTON button;
	STICK stick;
	GYRO gyro;
	INFO info;
}CONTROLLER;

class Sbdbt: public DualShock, public SerialHandler{
public:
	Sbdbt(Serial &serial, int baudrate = BAUDRATE);
	int setup();
	char* iam(){return name;};

	void serialReadChar(char data);

	void cycle();
	bool connection();
	bool battery();

	int getConnection();
	int getBattery();
	bool errorConnection();
	bool errorBattery();

	void setCrntState();
	void setPrevState();
	void setEdgeCon();

	//=== 十字キー
	//--- デジタル
	int getDigitalUp();
	int getDigitalDown();
	int getDigitalRight();
	int getDigitalLeft();
	//--- アナログ
	int getAnalogUp();
	int getAnalogDown();
	int getAnalogRight();
	int getAnalogLeft();
	//--- vlaue
	float getValueUp();
	float getValueDown();
	float getValueRight();
	float getValueLeft();
	//--- count
	int getCnUpEdgeUp();
	int getCnUpEdgeDown();
	int getCnUpEdgeRight();
	int getCnUpEdgeLeft();
	int getCnDnEdgeUp();
	int getCnDnEdgeDown();
	int getCnDnEdgeRight();
	int getCnDnEdgeLeft();

	//サイドボタン
	int getDigitalL1();
	int getDigitalL2();
	int getDigitalL3();
	int getDigitalR1();
	int getDigitalR2();
	int getDigitalR3();

	int getAnalogL1();
	int getAnalogL2();
	int getAnalogR1();
	int getAnalogR2();

	float getValueL1();
	float getValueL2();
	float getValueR1();
	float getValueR2();

	int getCnUpEdgeL1();
	int getCnUpEdgeL2();
	int getCnUpEdgeL3();
	int getCnUpEdgeR1();
	int getCnUpEdgeR2();
	int getCnUpEdgeR3();
	int getCnDnEdgeL1();
	int getCnDnEdgeL2();
	int getCnDnEdgeL3();
	int getCnDnEdgeR1();
	int getCnDnEdgeR2();
	int getCnDnEdgeR3();

	//記号ボタン
	int getDigitalTriangle();
	int getDigitalCross();
	int getDigitalCircle();
	int getDigitalRectangle();

	int getAnalogTriangle();
	int getAnalogCross();
	int getAnalogCircle();
	int getAnalogRectangle();

	float getValueTriangle();
	float getValueCross();
	float getValueCircle();
	float getValueRectangle();

	int getCnUpEdgeTriangle();
	int getCnUpEdgeCross();
	int getCnUpEdgeCircle();
	int getCnUpEdgeRectangle();
	int getCnDnEdgeTriangle();
	int getCnDnEdgeCross();
	int getCnDnEdgeCircle();
	int getCnDnEdgeRectangle();

	//=== オプションボタン
	int getDigitalPS();
	int getDigitalStart();
	int getDigitalSelect();

	//ジョイスティック
	int getStickLx();
	int getStickLy();
	int getStickRx();
	int getStickRy();

	int getAnalogLeftX();
	int getAnalogLeftY();
	int getAnalogRightX();
	int getAnalogRightY();

	float getValueLx();
	float getValueLy();
	float getValueRx();
	float getValueRy();

	float getStickRangle();
	float getStickLangle();
	float getStickRangle2();
	float getStickLangle2();

	float getAccelx();
	float getAccely();
	float getAccelx2();
	float getAccely2();

	float getValueAccelx();
	float getValueAccely();

	float getAngleAccel();

	//=== button
	//--- option
	 int home();
	 int ps();
	 int start();
	 int select();
	//--- arrow
	 float up();
	 float right();
	 float left();
	 float down();
	//--- side
	 float l1();
	 float l2();
	 float r1();
	 float r2();
	//--- simble
	 float triangle();
	 float circle();
	 float cross();
	 float square();
	//--- accel
	 float accelX();
	 float accelY();
	//--- stick
	 float lx();
	 float ly();
	 int l3();
	 float rx();
	 float ry();
	 int r3();

	 int disconnect();
	 int lowBattery();

	//=== 誤動作防止に閾値を設定
	 void setTolerance(float toleranceArg);

	 void setStickTolerance(float toleranceArg);

	 void setArrowKeyTolerance(float toleranceArg);

	 void setSymbolKeyTolerance(float toleranceArg);

	 void setSide1KeyTolerance(float toleranceArg);
	 void setSide2KeyTolerance(float toleranceArg);

	 void setOptionKeyTolerance(float toleranceArg);

	 void setlxTolerance(float toleranceArg);
	 void setlyTolerance(float toleranceArg);
	 void setrxTolerance(float toleranceArg);
	 void setryTolerance(float toleranceArg);

	 void setUpTolerance(float toleranceArg);
	 void setRightTolerance(float toleranceArg);
	 void setDownTolerance(float toleranceArg);
	 void setLeftTolerance(float toleranceArg);

	 void setTriangleTolerance(float toleranceArg);
	 void setCircleTolerance(float toleranceArg);
	 void setCrossTolerance(float toleranceArg);
	 void setSquareTolerance(float toleranceArg);

	 void setR1Tolerance(float toleranceArg);
	 void setR2Tolerance(float toleranceArg);
	 void setL1Tolerance(float toleranceArg);
	 void setL2Tolerance(float toleranceArg);

	 char _order[ORDER_NUM*10];
private:
	Serial *_serial;
	char name[32];

	int _toDebug;
	int _cnOrder;
	int _baudrate;
	float _prevGyroX;
	float _prevGyroY;
	float _prevGyroZ;
	int64_t _sameTime;
	bool _f_discon;
	bool _f_init;

	CONTROLLER _tolerance;
};
