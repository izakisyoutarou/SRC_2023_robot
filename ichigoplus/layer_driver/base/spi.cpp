#include "spi.hpp"


SpiHandler::SpiHandler(){
	optionPinEn=false;
}

int SpiHandler::spiHandlerSetup(Spi *spi){
	this->spi=spi;
	return 0;
}

int SpiHandler::spiSetup(){
	cs->setupDigitalOut();
	return spi->setup();
}

int SpiHandler::spiOptionPinSetup(){
	optionPinEn = true;
	return optionPin->setupDigitalOut();
}

int SpiHandler::spiWrite(int byteSize,unsigned char data[],bool optionPin){
	return spi->write(byteSize,data,this,optionPin);
}
