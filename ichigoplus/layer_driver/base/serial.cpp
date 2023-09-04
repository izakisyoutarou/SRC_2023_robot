#include "serial.hpp"
#include <stdio.h>
void defaultRxIntFunc(char)
{
    return;
}
void defaultTxIntFunc()
{
    return;
}

int SerialHandler::serialHandlerSetup(Serial *serial){
	this->serial=serial;return 0;
}

void SerialHandler::serialReadChar(char value)
{
    rxBuffer.write(value);
    if(value==DEFAULT_LINEFEED){
        char data[SERIAL_BUFFER_LENGTH];
        for(int i=0;i<SERIAL_BUFFER_LENGTH;i++){
            if(rxBuffer.isEmpty()){
                data[i]=0;
                break;
            }
            data[i]=rxBuffer.read();
            if(data[i]==DEFAULT_LINEFEED||i>=SERIAL_BUFFER_LENGTH-1){
                data[i]=0;
                serialReadString(data);
                break;
            }
        }
    }
}
void SerialHandler::serialPrintf(char *pFormat, ...)
{
    va_list ap;
    va_start(ap, pFormat);
    serialVprintf(pFormat,ap);
    va_end(ap);
}

void SerialHandler::serialVprintf(char *format,va_list arg){
	serial->vprintf(format,arg);
}

void SerialHandler::serialWriteChar(char value){
	serial->writeChar(value);
}
void SerialHandler::serialWriteString(char *value){
	serial->writeString(value);
}

int SerialHandler::serialSetup(int baudrate,int parity,int wordLength){
	//return
	serial->setup(baudrate,(*this),parity,wordLength);
	return 0;
}

void Serial::printf(const char *pFormat, ...)
{
    va_list ap;
    va_start(ap, pFormat);
    vprintf(pFormat,ap);
    va_end(ap);
}

void Serial::vprintf(const char *format,va_list arg)
{
    char buffer[SERIAL_BUFFER_LENGTH];
    vsprintf(buffer,format,arg);
    writeString(buffer);
}
