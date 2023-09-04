#include "sbdbt.hpp"
#include "ichigoplus/lib_src/utilplus.hpp"
#include <string.h>
#include <cmath>

using namespace rp_lib;

Sbdbt::Sbdbt(Serial &serial, int baudrate){
	serialHandlerSetup(&serial);
	_baudrate = baudrate;
	for(int i = 0; i < ORDER_NUM*10; i++)_order[i] = 0;
	_cnOrder = 0;
	_toDebug = 0;
	_prevGyroX = 0;
	setTolerance(0);
	_f_discon = 1;
	_f_init = 1;
	strcpy(name, NAME);
}

void Sbdbt::serialReadChar(char data){
	_f_init = 0;
	if(rxBuffer.isFull())rxBuffer.remove();

	_order[_cnOrder] = data;

	if(_cnOrder == 0 && data == 0x80){
		rxBuffer.remove();
		_cnOrder++;
		return;
	}else if(_cnOrder > 0){
		_cnOrder++;
	}

	if(_cnOrder >= ORDER_NUM)_cnOrder = 0;

	return;
}
int Sbdbt::setup(){
	return serialSetup(_baudrate);
}
void Sbdbt::cycle(){
	if(_prevGyroX == getAccelx() && _prevGyroY == getAccely()){
		if(millis() - _sameTime >= TIME_OUT || _f_init == 1){
			_f_discon = 1;
			for(int i = 0; i < ORDER_NUM+10; i++){
				if(i >= 3 && i <= 6)_order[i] = ANALOG_STICK_OFFSET;
				else _order[i] = 0;
			}
		}
	}else{
		_f_discon = 0;
		_sameTime = millis();
	}

	_prevGyroX = getAccelx();
	_prevGyroY = getAccely();
}


//=== コントローラステータス
int Sbdbt::getConnection(){return _order[CONNECTION_STATE];}
int Sbdbt::getBattery(){return _order[BATTERY_STATE];}

bool Sbdbt::connection(){
	return !_f_discon;
	//if(getConnection() > ERROR_CONNECTION)return 1;
	//return 0;
}

bool Sbdbt::battery(){
	if(getBattery() > ERROR_BATTERY)return 1;
	return 0;
}

bool Sbdbt::errorConnection(){
	if(getConnection() > ERROR_CONNECTION)return 0;
	return 1;
}

bool Sbdbt::errorBattery(){
	if(getBattery() > ERROR_BATTERY)return 1;
	return 0;
}

//===　十字キー
//--- デジタル
int Sbdbt::getDigitalUp(){
	if((_order[2] & DIGITAL_UP) == DIGITAL_UP)return 1;
	return 0;
}
int Sbdbt::getDigitalDown(){
	if((_order[2] & DIGITAL_DOWN) == DIGITAL_DOWN)return 1;
	return 0;
}
int Sbdbt::getDigitalRight(){
	if((_order[2] & DIGITAL_RIGHT) == DIGITAL_RIGHT)return 1;
	return 0;
}
int Sbdbt::getDigitalLeft(){
	if((_order[2] & DIGITAL_LEFT) == DIGITAL_LEFT)return 1;
	return 0;
}

//--- アナログ
//0~ff
int Sbdbt::getAnalogUp(){return _order[ANALOG_UP];}
int Sbdbt::getAnalogDown(){return _order[ANALOG_DOWN];}
int Sbdbt::getAnalogRight(){return _order[ANALOG_RIGHT];}
int Sbdbt::getAnalogLeft(){return _order[ANALOG_LEFT];}
//0~1
float Sbdbt::getValueUp(){return (float)getAnalogUp() / MAX_ANALOG_BUTTON;}
float Sbdbt::getValueDown(){return (float)getAnalogDown() / MAX_ANALOG_BUTTON;}
float Sbdbt::getValueRight(){return (float)getAnalogRight() / MAX_ANALOG_BUTTON;}
float Sbdbt::getValueLeft(){return (float)getAnalogLeft() / MAX_ANALOG_BUTTON;}

//=== サイドボタン
//--- デジタル
int Sbdbt::getDigitalR1(){
	if((_order[1] & (DIGITAL_R1 >> 8)) == (DIGITAL_R1 >> 8))return 1;
	return 0;
}
int Sbdbt::getDigitalR2(){
	if((_order[1] & (DIGITAL_R2 >> 8)) == (DIGITAL_R2 >> 8))return 1;
	return 0;
}
int Sbdbt::getDigitalL1(){
	if((_order[1] & (DIGITAL_L1 >> 8)) == (DIGITAL_L1 >> 8))return 1;
	return 0;
}
int Sbdbt::getDigitalL2(){
	if((_order[1] & (DIGITAL_L2 >> 8)) == (DIGITAL_L2 >> 8))return 1;
	return 0;
}
int Sbdbt::getDigitalR3(){
	if(!getDigitalPS() && (_order[1] & (DIGITAL_R3 >> 8)) == DIGITAL_R3 >> 8)return 1;
	return 0;
}
int Sbdbt::getDigitalL3(){
	if(!getDigitalPS() && (_order[1] & (DIGITAL_L3 >> 8)) == DIGITAL_L3 >> 8)return 1;
	return 0;
}
//--- アナログ
//0~ff
int Sbdbt::getAnalogR2(){return _order[ANALOG_R2];}
int Sbdbt::getAnalogR1(){return _order[ANALOG_R1];}
int Sbdbt::getAnalogL2(){return _order[ANALOG_L2];}
int Sbdbt::getAnalogL1(){return _order[ANALOG_L1];}
//0~1
float Sbdbt::getValueL1(){return (float)getAnalogL1() / MAX_ANALOG_BUTTON;}
float Sbdbt::getValueL2(){return (float)getAnalogL2() / MAX_ANALOG_BUTTON;}
float Sbdbt::getValueR1(){return (float)getAnalogR1() / MAX_ANALOG_BUTTON;}
float Sbdbt::getValueR2(){return (float)getAnalogR2() / MAX_ANALOG_BUTTON;}

//=== 記号キー
//---　デジタル
int Sbdbt::getDigitalTriangle(){
	if((_order[2] & DIGITAL_TRIANGLE) == DIGITAL_TRIANGLE)return 1;
	return 0;
}
int Sbdbt::getDigitalCross(){
	if((_order[2] & DIGITAL_CROSS) == DIGITAL_CROSS)return 1;
	return 0;
}
int Sbdbt::getDigitalCircle(){
	if((_order[2] & DIGITAL_CIRCLE) == DIGITAL_CIRCLE)return 1;
	return 0;
}
int Sbdbt::getDigitalRectangle(){
	if((_order[2] & DIGITAL_RECTANGLE) == DIGITAL_RECTANGLE)return 1;
	return 0;
}

//--- アナログ
//0~ff
int Sbdbt::getAnalogTriangle(){return _order[ANALOG_TRIANGLE];}
int Sbdbt::getAnalogCross(){return _order[ANALOG_CROSS];}
int Sbdbt::getAnalogCircle(){return _order[ANALOG_CIRCLE];}
int Sbdbt::getAnalogRectangle(){return _order[ANALOG_RECTANGLE];}
//0~1
float Sbdbt::getValueTriangle(){return (float)getAnalogTriangle() / MAX_ANALOG_BUTTON;}
float Sbdbt::getValueCross(){return (float)getAnalogCross() / MAX_ANALOG_BUTTON;}
float Sbdbt::getValueCircle(){return (float)getAnalogCircle() / MAX_ANALOG_BUTTON;}
float Sbdbt::getValueRectangle(){return (float)getAnalogRectangle() / MAX_ANALOG_BUTTON;}

//=== オプションボタン
int Sbdbt::getDigitalPS(){
	if((_order[1] & (DIGITAL_PS >> 8)) == (DIGITAL_PS >> 8))return 1;
	return 0;
}
int Sbdbt::getDigitalStart(){
	if(!getDigitalPS() && (_order[1] & (DIGITAL_START >> 8)) == (DIGITAL_START >> 8))return 1;
	return 0;
}
int Sbdbt::getDigitalSelect(){
	if(!getDigitalPS() && (_order[1] & (DIGITAL_SELECT >> 8)) == (DIGITAL_SELECT >> 8))return 1;
	return 0;
}

//=== ジョイスティック
//--- 素データ
int Sbdbt::getAnalogLeftX(){return _order[3];}
int Sbdbt::getAnalogLeftY(){return _order[4];}
int Sbdbt::getAnalogRightX(){return _order[5];}
int Sbdbt::getAnalogRightY(){return _order[6];}
//--- 中心を0としたデータ
/*			     |y+(64)
 * 			     |
 * 				 |
 * (-64)x- ------o------x+(63)
 * 			     |
 * 			     |
 * 			     |y-(-63)
 */
/*
int Sbdbt::getStickLx(){return getAnalogLeftX() - ANALOG_STICK_OFFSET;}
int Sbdbt::getStickLy(){return (getAnalogLeftY() - ANALOG_STICK_OFFSET) * -1;}
int Sbdbt::getStickRx(){return getAnalogRightX() - ANALOG_STICK_OFFSET;}
int Sbdbt::getStickRy(){return (getAnalogRightY() - ANALOG_STICK_OFFSET) * -1;}
*/

//--- 中心を0としたデータ、getStick()の戻り値
/*			     |x+(64)
 * 			     |
 * 				 |
 * (64)y- -------o------y+(-63)
 * 			     |
 * 			     |
 * 			     |x-(-63)
 */
int Sbdbt::getStickLx(){return (getAnalogLeftY() - ANALOG_STICK_OFFSET) * -1;}
int Sbdbt::getStickLy(){return (getAnalogLeftX() - ANALOG_STICK_OFFSET) * -1;}
int Sbdbt::getStickRx(){return (getAnalogRightY() - ANALOG_STICK_OFFSET) * -1;}
int Sbdbt::getStickRy(){return (getAnalogRightX() - ANALOG_STICK_OFFSET) * -1;}


//スティック入力時に遅延が発生するのでコントローラーの出力値を変更した
float Sbdbt::getValueLx(){
	if(getStickLx() > 0)return floatlimit(-0.35f, (float)getStickLx() / 64, 0.35f);
	else if(getStickLx() < 0)return floatlimit(-0.35f, (float)getStickLx() / 63, 0.35f);
	return 0;
}
float Sbdbt::getValueLy(){
	if(getStickLy() > 0)return floatlimit(-0.35f, (float)getStickLy() / 64, 0.35f);
	else if(getStickLy() < 0)return floatlimit(-0.35f, (float)getStickLy() / 63, 0.35f);
	return 0;
}
float Sbdbt::getValueRx(){
	if(getStickRx() > 0)return floatlimit(-1.f, (float)getStickRx() / 64, 1.f);
	else if(getStickRx() < 0)return floatlimit(-1.f, (float)getStickRx() / 63, 1.f);
	return 0;
}
float Sbdbt::getValueRy(){
	if(getStickRy() > 0)return floatlimit(-1.f, (float)getStickRy() / 64, 1.f);
	else if(getStickRy() < 0)return floatlimit(-1.f, (float)getStickRy() / 63, 1.f);
	return 0;
}

//--- -90~+90
//	左回転角度:+
//	右回転角度:-
float Sbdbt::getStickLangle(){return -std::atan2(ly(), lx());}
float Sbdbt::getStickRangle(){return -std::atan2(ry(), rx());}

//--- 0~+180
//	左回転角度
float Sbdbt::getStickLangle2(){
	if(getStickLangle() < 0)return getStickLangle()+2*f_pi;
	return getStickLangle();
}
float Sbdbt::getStickRangle2(){
	if(getStickRangle() < 0)return getStickRangle()+2*f_pi;
	return getStickRangle();
}

//=== コントローラジャイロ
//--- -90~+90
float Sbdbt::getAccelx(){
	float value;
	float angle = 0;

	value = (float)_order[ANALOG_ACCEL_X_L] + ((float)(_order[ANALOG_ACCEL_X_H] << 8));

	if(value > MID_ACCEL_X){
		angle = ((value - MAX_ACCEL_X) / (MAX_ACCEL_X - MID_ACCEL_X) + 1) * (f_pi / 2);
	}else if(value < MID_ACCEL_X){
		angle = ((MIN_ACCEL_X - value) / (MIN_ACCEL_X - MID_ACCEL_X) - 1) * (f_pi / 2);
	}else{
		angle = 0;
	}
	//return value;
	return angle;
}

float Sbdbt::getAccely(){
	float value;
	float angle = 0;

	value = (float)_order[ANALOG_ACCEL_Y_L] + ((float)(_order[ANALOG_ACCEL_Y_H] << 8));

	if(value > MID_ACCEL_Y){
		angle = ((value - MAX_ACCEL_Y) / (MAX_ACCEL_Y - MID_ACCEL_Y) + 1) * (f_pi / 2);
	}else if(value < MID_ACCEL_X){
		angle = ((MIN_ACCEL_Y - value) / (MIN_ACCEL_Y - MID_ACCEL_Y) - 1) * (f_pi / 2);
	}else{
		angle = 0;
	}

	return angle;
}

//--- 0~360
float Sbdbt::getAccelx2(){
	float value;
	float angle = 0;

	value = (float)_order[ANALOG_ACCEL_X_L] + ((float)(_order[ANALOG_ACCEL_X_H] << 8));
	angle = (value - MIN_ACCEL_X) / ((MAX_ACCEL_X - MIN_ACCEL_X) / f_pi);

	return angle;
}
float Sbdbt::getAccely2(){
	float value;
	float angle = 0;

	value = (float)_order[ANALOG_ACCEL_Y_L] + ((float)(_order[ANALOG_ACCEL_Y_H] << 8));
	angle = (value - MIN_ACCEL_Y) / ((MAX_ACCEL_Y - MIN_ACCEL_Y) / f_pi);

	return angle;
}

float Sbdbt::getValueAccelx(){
	float value = getAccelx();

	if(value > 0){
		if(value > (f_pi / 2))return 1;
		return value / (f_pi / 2);
	}else if(value < 0){
		if(value < -1 * f_pi / 2)return -1;
		return value / (f_pi / 2);
	}
	return 0;
}

float Sbdbt::getValueAccely(){
	float value = getAccely();

	if(value > 0){
		if(value > (f_pi / 2))return 1;
		return value / (f_pi / 2);
	}else if(value < 0){
		if(value < -1 * f_pi / 2)return -1;
		return value / (f_pi / 2);
	}
	return 0;
}

float Sbdbt::getAngleAccel(){
	return std::atan2(getValueAccelx(), getValueAccely())*-1 + f_pi;
}

//=== button
//--- option
 int Sbdbt::home(){return getDigitalPS();}
 int Sbdbt::ps(){return getDigitalPS();}
 int Sbdbt::start(){return getDigitalStart();}
 int Sbdbt::select(){return getDigitalSelect();}

//--- arrow
float Sbdbt::up(){
	if(_tolerance.button.up < getValueUp())return getValueUp();
	else return 0;
}

float Sbdbt::right(){
	if(_tolerance.button.right < getValueRight())return getValueRight();
	else return 0;
}
float Sbdbt::left(){
	if(_tolerance.button.left < getValueLeft())return getValueLeft();
	else return 0;
}
float Sbdbt::down(){
	if(_tolerance.button.down < getValueDown())return getValueDown();
	else return 0;
}

//--- side
float Sbdbt::l1(){
	if(_tolerance.button.l1 < getValueL1())return getValueL1();
	else return 0;
}
float Sbdbt::l2(){
	if(_tolerance.button.l2 < getValueL2())return getValueL2();
	else return 0;
}
float Sbdbt::r1(){
	if(_tolerance.button.r1 < getValueR1())return getValueR1();
	else return 0;
}
float Sbdbt::r2(){
	if(_tolerance.button.r2 < getValueR2())return getValueR2();
	else return 0;
}
//--- simble
float Sbdbt::triangle(){
	if(_tolerance.button.triangle < getValueTriangle())return getValueTriangle();
	else return 0;
}
float Sbdbt::circle(){
	if(_tolerance.button.circle < getValueCircle())return getValueCircle();
	else return 0;
}
float Sbdbt::cross(){
	if(_tolerance.button.cross < getValueCross())return getValueCross();
	else return 0;
}
float Sbdbt::square(){
	if(_tolerance.button.rectangle < getValueRectangle())return getValueRectangle();
	else return 0;
}
//--- accel
float Sbdbt::accelX(){return getAccelx();}
float Sbdbt::accelY(){return getAccely();}

//--- stick
float Sbdbt::lx(){
	if(std::abs(_tolerance.stick.lx) < std::abs(getValueLx()))return getValueLx();
	return 0;
}
float Sbdbt::ly(){
	if(std::abs(_tolerance.stick.ly) < std::abs(getValueLy()))return getValueLy();
	return 0;
}
int Sbdbt::l3(){return getDigitalL3();}
float Sbdbt::rx(){
	if(std::abs(_tolerance.stick.rx) < std::abs(getValueRx()))return getValueRx();
	return 0;
}
float Sbdbt::ry(){
	if(std::abs(_tolerance.stick.ry) < std::abs(getValueRy()))return getValueRy();
	return 0;
}
int Sbdbt::r3(){return getDigitalR3();}
int Sbdbt::disconnect(){return !connection();}
int Sbdbt::lowBattery(){return !battery();}

void Sbdbt::setTolerance(float toleranceArg){
	setStickTolerance(toleranceArg);
	setArrowKeyTolerance(toleranceArg);
	setSymbolKeyTolerance(toleranceArg);
	setSide1KeyTolerance(toleranceArg);
	setSide2KeyTolerance(toleranceArg);
}
//=== 誤動作防止に閾値を設定
void Sbdbt::setStickTolerance(float toleranceArg){
	_tolerance.stick.lx = _tolerance.stick.ly =
		 _tolerance.stick.rx = _tolerance.stick.ry = toleranceArg;
	return;
}

void Sbdbt::setArrowKeyTolerance(float toleranceArg){
	_tolerance.button.up = _tolerance.button.right =
			_tolerance.button.down = _tolerance.button.left = toleranceArg;
	return;
}

void Sbdbt::setSymbolKeyTolerance(float toleranceArg){
	_tolerance.button.triangle = _tolerance.button.circle =
			_tolerance.button.cross = _tolerance.button.rectangle = toleranceArg;
	return;
}

void Sbdbt::setSide1KeyTolerance(float toleranceArg){
	_tolerance.button.l1 = _tolerance.button.r1 = toleranceArg;
}
void Sbdbt::setSide2KeyTolerance(float toleranceArg){
	_tolerance.button.l2 = _tolerance.button.r2 = toleranceArg;
}
void Sbdbt::setOptionKeyTolerance(float toleranceArg){
	_tolerance.button.ps = _tolerance.button.start = _tolerance.button.select = toleranceArg;
}

void Sbdbt::setlxTolerance(float toleranceArg){_tolerance.stick.lx = toleranceArg;}
void Sbdbt::setlyTolerance(float toleranceArg){_tolerance.stick.ly = toleranceArg;}
void Sbdbt::setrxTolerance(float toleranceArg){_tolerance.stick.rx = toleranceArg;}
void Sbdbt::setryTolerance(float toleranceArg){_tolerance.stick.ry = toleranceArg;}

void Sbdbt::setUpTolerance(float toleranceArg){_tolerance.button.up = toleranceArg;}
void Sbdbt::setRightTolerance(float toleranceArg){_tolerance.button.right = toleranceArg;}
void Sbdbt::setDownTolerance(float toleranceArg){_tolerance.button.down = toleranceArg;}
void Sbdbt::setLeftTolerance(float toleranceArg){_tolerance.button.left = toleranceArg;}

void Sbdbt::setTriangleTolerance(float toleranceArg){_tolerance.button.triangle = toleranceArg;}
void Sbdbt::setCircleTolerance(float toleranceArg){_tolerance.button.circle = toleranceArg;}
void Sbdbt::setCrossTolerance(float toleranceArg){_tolerance.button.cross = toleranceArg;}
void Sbdbt::setSquareTolerance(float toleranceArg){_tolerance.button.rectangle = toleranceArg;}

void Sbdbt::setR1Tolerance(float toleranceArg){_tolerance.button.l1 = toleranceArg;}
void Sbdbt::setR2Tolerance(float toleranceArg){_tolerance.button.l2 = toleranceArg;}
void Sbdbt::setL1Tolerance(float toleranceArg){_tolerance.button.r1 = toleranceArg;}
void Sbdbt::setL2Tolerance(float toleranceArg){_tolerance.button.r2 = toleranceArg;}


