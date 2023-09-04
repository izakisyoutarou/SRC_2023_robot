#pragma once

class Eeprom{
public:
    virtual int setup()=0;
    virtual void cycle(){};
    virtual void write(unsigned char *writeAddress,unsigned char *data,int dataLength)=0;
    virtual void writeChar(unsigned char *writeAddress,char data)=0;
    virtual void writeInt(unsigned char *writeAddress,int data)=0;
    virtual void writeFloat(unsigned char *writeAddress,float data)=0;
    virtual char readChar(unsigned char *readAddress)=0;
    virtual int readInt(unsigned char *readAddress)=0;
    virtual float readFloat(unsigned char *readAddress)=0;
};
