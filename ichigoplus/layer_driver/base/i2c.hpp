#pragma once

#define TX true
#define RX false

class I2c;
class I2cHandler{
public:
	int i2cSetup();
	int i2cWrite(char address,unsigned char *value,char dataSize,bool txrx);
	virtual void i2cRead(unsigned char *data)=0;
	virtual void i2cReStartSend(char data)=0;
	virtual int i2cAddress(int address){return 0;};
	int i2cHandlerSetup(I2c *i2c);

private:
	I2c *i2c;
};

class I2c{
public:
	virtual int setup()=0;
	virtual int write(char address,unsigned char *value,char dataSize,bool txrx)=0;
	virtual int addHandler(I2cHandler &handlerArg)=0;

	virtual void setRx(int rxBufferSize,int rxSlaveAddress)=0;
	virtual bool direction()=0;

	int addHandler(I2cHandler *handlerArg){return addHandler(*handlerArg);};
};

class I2cHandlerNull:public I2cHandler{
public:
	virtual void i2cRead(unsigned char *data){};
	virtual void i2cReStartSend(char data){};
};
#include <stdio.h>
class I2cNull:public I2c{
public:
	bool printfAvailable;
	int setupReturnValue;
	int writeReturnValue;
	int addHandlerReturnValue;
	bool directionReturnValue;
	I2cNull(bool printfAvailable=true){
		this->printfAvailable=printfAvailable;
		writeReturnValue=0;
		addHandlerReturnValue=0;
		directionReturnValue=false;
		setupReturnValue=0;
	}
	virtual int setup(){
		if(printfAvailable)printf("I2c::setup\n");
		return setupReturnValue;
	}
	virtual int write(char address,unsigned char *value,char dataSize,bool txrx){
		return writeReturnValue;
	}
	virtual int addHandler(I2cHandler &handlerArg){
		return addHandlerReturnValue;
	}

	virtual void setRx(int rxBufferSize,int rxSlaveAddress){
	}
	virtual bool direction(){
		return directionReturnValue;
	}
};
