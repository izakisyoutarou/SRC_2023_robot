#pragma once

class Potentio{
private:
	float originValue;
protected:
	float revolutionValue;
public:
	void setOrigin(float originValue);					//原点の設定
	float origin();								      	//設定した原点の値を返す
	float revolution();						     	 	//最大の回転角を返す
	virtual int setup()=0;
	virtual float read()=0;								//ポテンショの角度を返す
	virtual int resolution()=0;							//分解能を返す。
};
