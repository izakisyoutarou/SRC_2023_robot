#include "i2c.hpp"

int I2cHandler::i2cHandlerSetup(I2c *i2c){
	this->i2c=i2c;
	return 0;
}

int I2cHandler::i2cSetup(){
	return i2c->setup();
}

int I2cHandler::i2cWrite(char address,unsigned char *value,char dataSize,bool txrx){
	return i2c->write(address,value,dataSize,txrx);
}
