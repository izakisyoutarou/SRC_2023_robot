#include "can.hpp"
int CanHandler::canHandlerSetup(Can *can){
	this->can=can;
	return 0;
}

int CanHandler::canSetup(){
	return can->setup();
}

int CanHandler::canWrite(int idArg,int numberArg,unsigned char dataArg[8]){
	return can->write(idArg,numberArg,dataArg);
}

int CanHandler::canSetId(int id){
	return can->setId(id);
};

int CanHandler::canSetIdAll(){
	return can->setIdAll();
}

int CanHandler::canTxBufLength(){
	return can->txBufLength();
}

void CanHandler::canSetTxTimeOut(int time){
	can->setTxTimeOut(time);
}

int CanHandler::canReceiveErrorCounter(){
	return can->receiveErrorCounter();
}

int CanHandler::canTransmitErrorCounter(){
	return can->transmitErrorCounter();
}
