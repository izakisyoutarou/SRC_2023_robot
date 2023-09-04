#pragma once

class Analog{
//***************override******************
protected:
    virtual int _setupAnalogIn()=0;
    virtual float _analogRead()=0;
public:
    virtual int analogResolution()=0;
    virtual float analogVoltage()=0;
//***************override******************
    virtual int setupAnalogIn();
    virtual float analogRead();

    operator float() {
        return analogRead();
    }
};
#include <stdio.h>
class AnalogNull:public Analog{
public:
	AnalogNull(bool printfAvailable){
		analogValue=0.0f;
		setupReturnValue=0;
		analogResolutionReturnValue=1024;
		analogVoltageReturnValue=5.0f;
		this->printfAvailable=printfAvailable;
	}
	float analogValue;
	int setupReturnValue;
	bool printfAvailable;
	int analogResolutionReturnValue;
	float analogVoltageReturnValue;

    virtual int _setupAnalogIn(){
    	if(printfAvailable)printf("AnalogNull::setup\n");
    	return setupReturnValue;
    };
    virtual float _analogRead(){return analogValue;};
    virtual int analogResolution(){return analogResolutionReturnValue;};
    virtual float analogVoltage(){return analogVoltageReturnValue;};
};
