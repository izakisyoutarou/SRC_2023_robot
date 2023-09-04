#pragma once

#include "ichigoplus/layer_driver/base/analog.hpp"
#include "potentio.hpp"

class Bourns1:public Potentio{
private:
	Analog &analog;
public:
	Bourns1(Analog &pin);	           //最大角を(3/4)*2*f_piに設定。実は1回転ではない
	int setup();                       //analogのsetup
	int resolution();                  //基底クラスのrevolutionと間違えないように注意
	float read();                      //角度を返す[rad]
};
