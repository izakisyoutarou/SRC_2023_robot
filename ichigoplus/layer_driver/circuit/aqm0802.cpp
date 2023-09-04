#include "aqm0802.hpp"

#include "ichigoplus/layer_driver/base/mcutime.h"
#include <string.h>

using namespace std;


Aqm0802::Aqm0802(I2c &i2cPin){
	address=0x7C;
	i2cPin.addHandler(this);

	time=0;
	num=0;
	count=0;
	floor=OVER;

	x=0;
	y=0;

	upX=0;
	downX=0;
	memset( up , '\0' , sizeof(up));
	memset( down , '\0' , sizeof(down));
	memset( str , '\0' , sizeof(str));
	memset( size , '\0' , sizeof(size));
}

int Aqm0802::setup(){
	unsigned char data[3]={0};
	i2cSetup();
	data[0]=0x00;
	data[1]=0x38;
	i2cWrite(0x7C,data,2,TX);delay_ms(2);
	data[1]=0x39;
	i2cWrite(0x7C,data,2,TX);delay_ms(2);
	data[1]=0x14;
	i2cWrite(0x7C,data,2,TX);delay_ms(2);
	data[1]=0x70;
	i2cWrite(0x7C,data,2,TX);delay_ms(2);
	data[1]=0x56;
	i2cWrite(0x7C,data,2,TX);delay_ms(2);
	data[1]=0x6C;
	i2cWrite(0x7C,data,2,TX);delay_ms(2);
	data[1]=0x38;
	i2cWrite(0x7C,data,2,TX);delay_ms(2);
	data[1]=0x0C;
	i2cWrite(0x7C,data,2,TX);delay_ms(2);
	data[1]=0x01;
	i2cWrite(0x7C,data,2,TX);delay_ms(10);
	return 0;
}

void Aqm0802::sendString(const char data[WORD]){
	int j=0;
	for(int i=0;*(data+i)!='\0';i++){
		if(!j)str[num][0]=0x40;
		str[num][j+1]=*(data+i);
		if(!y){
			up[x]=*(data+i);
			if(x < 6)x++;
		}else if(y){
			down[x]=*(data+i);
			if(x < 6)x++;
		}
		j++;
		count++;
		if(count >= 8){
			size[num]=(int)strlen((char*)str[num]);
			if(num < MAX-1)num++;
			if(floor==OVER){
				setCursor(0,1);
				floor=UNDER;
			}else{
				setCursor(0,0);
				floor=OVER;
			}
			j=0;
		}
	}
	if(j){
		size[num]=(int)strlen((char*)str[num]);
		if(num < MAX-1)num++;
		//else num=0;
	}
}

void Aqm0802::sendChar(const char data){
	str[num][0]=0x40;
	str[num][1]=data;

	size[num]=2;
	if(!y){
		up[x]=data;
		if(x < 6)x++;
	}else if(y){
		down[x]=data;
		if(x < 6)x++;
	}
	if(num < MAX-1)num++;
	count++;
	if(count >= 8){
		count=0;
		if(floor==OVER){
			setCursor(0,1);
			floor=UNDER;
		}else{
			setCursor(0,0);
			floor=OVER;
		}
	}
	//else num=0;
}

void Aqm0802::setCursor(const char x,const char y){
	this->x=x;
	this->y=y;
	str[num][0]=0x00;
	str[num][1]=0x80|y*0x40|x;
	size[num]=2;
	count=0;

	if(num < MAX-1)num++;
	//else num=0;
}

void Aqm0802::clear(){
	this->x=0;
	this->y=0;
	count=0;
	floor=OVER;
	str[num][0]=0x00;
	str[num][1]=0x01;
	size[num]=2;
	if(num < MAX-1)num++;
	//else num=0;
}

void Aqm0802::cycle(){

	static int i=0;
	if(i<num){
		if(millis()-time >=10){
			time=millis();

			i2cWrite(0x7C,str[i],size[i],TX);

			memset( str[i] , '\0' , size[i]);
			size[i]=0;

			if(i < MAX-1)i++;
			else i=0;
		}
	}else{
		num=0;
		i=0;
	}
}
