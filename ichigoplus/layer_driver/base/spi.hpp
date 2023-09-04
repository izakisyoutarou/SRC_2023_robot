#pragma once
/**************************************************
 * spi ver1.1 2020/2/20
 * Spi, SpiHandler class.
 *
 * [Author]
 * Taro Yamada
 *
 * [Details]
 * SPI base class
 *
 * [Dependency]
 * digital.hpp"
 *
 * [Change history]
 * ver1.1 2020/ 2/ 20 bug fix
 * ver1.0 2018/ x/ xx The first version.
 **************************************************/

#include "digital.hpp"

class Spi;
class SpiHandler{
public:
	SpiHandler();
	int spiSetup();
	int spiOptionPinSetup();
	int spiWrite(int byteSize,unsigned char data[],bool optionPin=0);
	virtual int spiRead(int byteSize,unsigned char data[])=0;
	int spiHandlerSetup(Spi *spi);
	Digital *cs;
	Digital *optionPin;
	bool optionPinEn;
private:

	Spi *spi;


};

class Spi{
public:
	virtual int setup()=0;
	virtual int write(int byteSize,unsigned char data[],SpiHandler *HandlerArg,bool optionPin=0)=0;
	virtual int addHandler(SpiHandler &handlerArg)=0;

	int addHandler(SpiHandler *handlerArg){return addHandler(*handlerArg);};
};

class SpiHandlerNull:public SpiHandler{
public:
	virtual void spiRead(unsigned char *data){};
	virtual void spiReStartSend(char data){};
};
