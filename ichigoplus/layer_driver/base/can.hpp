#pragma once

/*******************************************
 * can ver2.0 2015/10/8
 * Base classes of Controller Area Network communication.
 * CanHandler declaration.
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Change history]
 * ver2.0 2015/10/ 9 improve dependency.
 * ver1.0 2015/ 3/ 3 The first version.
 ******************************************/

class Can;
class CanHandler{
public:
	int canSetup();
	int canWrite(int idArg,int numberArg,unsigned char dataArg[8]);
	int canSetId(int id);
	int canSetIdAll();
	int canTxBufLength();
	int canReceiveErrorCounter();
	int canTransmitErrorCounter();
	void canSetTxTimeOut(int time);
	virtual int canRead(int id,int number,unsigned char data[8])=0;
	virtual int canId(int id){return 0;};

	int canHandlerSetup(Can *can);
private:
	Can *can;
};

class Can{
public:
	virtual int setup()=0;
	virtual int setupLoopBack()=0;
	int addHandler(CanHandler *handlerArg){return addHandler(*handlerArg);};
	virtual int addHandler(CanHandler &handlerArg)=0;
	virtual int setId(int id)=0;
	virtual int setIdAll()=0;
	virtual int write(int id,int number,unsigned char data[8])=0;
	virtual int txBufLength(){return 0;};
	virtual void setTxTimeOut(int time){};
	virtual int receiveErrorCounter(){return -1;};
	virtual int transmitErrorCounter(){return -1;};

};

#include <stdio.h>
#include <string.h>
class CanNull:public Can{
public:
	CanNull(bool printfAvailable=true){setupValue=0;handlerValue=0;writeValue=0;this->printfAvailable=printfAvailable;};
	int setupValue;
	int handlerValue;
	int writeValue;
	bool printfAvailable;

	virtual int setup(){
		if(printfAvailable)printf((char*)"CanNull::setup\n");
		return setupValue;
	};
	virtual int setupLoopBack(){return setupValue;};
	virtual int addHandler(CanHandler &handlerArg){handlerArg.canHandlerSetup(this);return handlerValue;};
	virtual int setId(int id){return 0;};
	virtual int setIdAll(){return 0;};
	virtual int write(int id,int number,unsigned char data[8]){return writeValue;};
	virtual int txBufLength(){return 0;};
};
class CanHandlerNull:public CanHandler{
public:
	bool printfAvailable;
	CanHandlerNull(){
		printfAvailable=false;
	};

	virtual int canRead(int id,int number,unsigned char data[8]){
		if(printfAvailable)printf("chn,0x%x,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n"
				,id,number,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]);
		return 0;
	};

};
