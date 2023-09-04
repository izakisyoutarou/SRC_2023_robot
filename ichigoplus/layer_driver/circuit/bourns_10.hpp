#pragma once

#include "ichigoplus/layer_driver/base/analog.hpp"
#include "potentio.hpp"

class Bourns10:public Potentio{
private:
	Analog &analog;
public:
	Bourns10(Analog &pin);	            //最大角を10*2*f_piに設定
	int setup();				        //analogをsetup
	int resolution();                   //基底クラスのrevolutionと間違えないように注意
	float read();                       //角度を返す[rad]
};
