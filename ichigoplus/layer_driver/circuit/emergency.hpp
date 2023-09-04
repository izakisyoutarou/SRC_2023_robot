#pragma once
#include "ichigoplus/layer_driver/base/digital.hpp"

/***仕様***
 * 緊急停止かける場合:emergencyPinの出力をLowにする.
 * 緊急停止スイッチが押されている場合:readPinはtrueを返す.押されていない場合はfalseを返す.
 */

class Emergency {
private:
	Digital *emergencyPin;
	Digital *readPin;
	bool state;
	int emergencyState;
public:
	Emergency(Digital &emergencyPin, Digital &readPin);
	int setup();
	void cycle();	//制御周期外で回すこと.
	bool readOutside();	//緊急停止スイッチが押されているか否か(main基板以外から緊急停止がされているか)
	bool readInside();	//main基板自身で緊急停止信号をしているかどうか
	void on();	//緊急停止
	void off();	//緊急停止解除
	int a();	//メイン基板から緊急停止信号をかけているかどうかを返す
};
