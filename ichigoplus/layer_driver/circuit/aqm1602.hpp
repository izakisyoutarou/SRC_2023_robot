#pragma once

#include "ichigoplus/layer_driver/base/i2c.hpp"

#define MAX 250
#define WORD 200

class Aqm1602:public I2cHandler{
private:
	char address;

	int64_t time;
	int num;
	int upX;
	int upY;
	int downX;
	int downY;
	int count;
	int floor;

	int x;
	int y;


public:
	enum{
		OVER=0,
		UNDER,
	};
	int size[MAX];
	unsigned char str[MAX][WORD];
	unsigned char up[8];
	unsigned char down[8];

	Aqm1602(I2c &i2cPin);
	int setup();
	void sendString(const char *data);
	void sendChar(const char data);
	void setCursor(const char x,const char y);
	void clear();

	void sends(const char *data);
	void cycle();
	/*read���Ȃ��̂ŕK�v�Ȃ�*/
	void i2cRead(unsigned char *data){};
	void i2cReStartSend(char data){};
	//unsigned char *ret(){return ;};

};
