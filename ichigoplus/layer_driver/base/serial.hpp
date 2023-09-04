#pragma once
/*******************************************
 * usartplus ver2.0 2015/3/10
 * Classes of usart for stm32f4 discovery 2-layer board.
 * Serial handler declaration.
 *
 * [Dependency]
 * usart
 * if(STM32F4DISCOVERY_2LB_3_6) encoderplus
 *
 * [Note]
 *
 * [Change history]
 * ver2.1 2015/ 3/12 At STM32F4D_2LB ver.3.6 enc2 can use as serial communication port.
 * ver2.0 2015/ 3/10 Rename usart to serial.Multiple boards available.
 * ver1.0 2015/ 3/ 3 The first version.
 ******************************************/
#ifdef __cplusplus

#include "ichigoplus/lib_src/utilplus.hpp"
#include <stdarg.h>
#define DEFAULT_LINEFEED '\r'
#define SERIAL_BUFFER_LENGTH 512


class Serial;
class SerialHandler{
protected:
    rp_lib::RingBuffer<char,SERIAL_BUFFER_LENGTH> rxBuffer;
    Serial *serial;
    enum{
        	SERIAL_PARITY_NONE,
        	SERIAL_PARITY_ODD,
        	SERIAL_PARITY_EVEN
        };
	int serialSetup(int baudrate,int parity=SERIAL_PARITY_NONE,int wordLength=8);
public:
	virtual void serialWriteChar(char value);
	virtual void serialWriteString(char *value);
	virtual void serialPrintf(char *pFormat, ...);
	virtual void serialVprintf(char *format,va_list arg);

	int serialHandlerSetup(Serial *serial);
	virtual void serialReadChar(char value);
	virtual void serialReadString(char *data){};
};

class Serial{
private:
    class Receiver:public SerialHandler{
    public:
//        RingBuffer<char,SERIAL_BUFFER_LENGTH> rxBuffer;
        int stringAvailavleData;
        Serial *master;
        Receiver(Serial *serial){
            master=serial;
            stringAvailavleData=0;
        };
        void serialReadChar(char value){
        	if(!rxBuffer.write(value)){
        		if(value==DEFAULT_LINEFEED) stringAvailavleData++;
        	}else{
        	    if(stringAvailavleData==0&&value==DEFAULT_LINEFEED)stringAvailavleData=1;
        	}
        };
        int charAvailable(){
        	return (!rxBuffer.isEmpty());
        };
        int stringAvailable(){
        	return stringAvailavleData;
        };
        char readChar(){
        	return rxBuffer.read();
        };
        int readString(char *value){
            if(rxBuffer.isEmpty()){
                value[0]=0;
                stringAvailavleData=0;
                return 0;
            }
        	for(int i=0;i<256;i++){
        		value[i]=rxBuffer.read();
        		if(value[i]==DEFAULT_LINEFEED||i>=SERIAL_BUFFER_LENGTH-1){
        		    value[i]=0;
                    if(rxBuffer.isEmpty()) stringAvailavleData=0;
                    else stringAvailavleData--;
                    return i;
        		}
        		if(rxBuffer.isEmpty()){
                    value[i]=0;
                    stringAvailavleData=0;
                    return i;
                }
        	}
        	return 0;
        };
    };
    Receiver *defaultHandler;
    int _serial_mode;
    enum{
        SERIAL_MODE_NULL,
        SERIAL_MODE_LOCAL,
        SERIAL_MODE_HANDLER
    };
protected:
    virtual int _setup(int baudrate,SerialHandler &handlerArg,int parity,int wordLength)=0;
public:
    enum{
    	SERIAL_PARITY_NONE,
    	SERIAL_PARITY_ODD,
    	SERIAL_PARITY_EVEN
    };
	virtual void writeChar(char value)=0;
	virtual int stringAvailable(){
		if(_serial_mode==SERIAL_MODE_LOCAL) return defaultHandler->stringAvailable();
		else return 0;
	};
	virtual int readString(char *value){
		value[0]=0;
		if(_serial_mode==SERIAL_MODE_LOCAL)return defaultHandler->readString(value);
		else return 0;
	};
	virtual int charAvailable(){
		if(_serial_mode==SERIAL_MODE_LOCAL)return defaultHandler->charAvailable();
		else return 0;
	};
	virtual char readChar(){
		if(_serial_mode==SERIAL_MODE_LOCAL)return defaultHandler->readChar();
		else return 0;
	};

	virtual void writeString(char *value){for(int i=0;value[i]!='\0';i++) writeChar(value[i]);};
	virtual void printf(const char *pFormat, ...);
	virtual void vprintf(const char *format,va_list arg);
	virtual int setup(int baudrate,SerialHandler &handlerArg,int parity=SERIAL_PARITY_NONE,int wordLength=8){_serial_mode=SERIAL_MODE_HANDLER;return _setup(baudrate,handlerArg,parity,wordLength);};
	virtual int setup(int baudrate,int parity=SERIAL_PARITY_NONE,int wordLength=8){_serial_mode=SERIAL_MODE_LOCAL;defaultHandler=new Receiver(this);return _setup(baudrate,*defaultHandler,parity,wordLength);};
};

#include <stdio.h>
class SerialHandlerNull:public SerialHandler{
public:

};

class SerialNull:public Serial{
public:
	bool printfAvailable;
	int setupReturnValue;
	SerialNull(bool printfAvailable=true){this->printfAvailable=printfAvailable;setupReturnValue=0;}
    virtual int _setup(int baudrate,SerialHandler &handlerArg,int parity,int wordLength){
    	return setupReturnValue;
    }
	virtual void writeChar(char value){
		return ;
	}
};

#endif//__cplusplus
