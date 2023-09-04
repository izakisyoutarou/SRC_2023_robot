#pragma once
/*******************************************
 * digital ver4.0 2019/11/12
 * Digital class.
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Change history]
 * ver4.0 2019/11/12 change variable access modifier to protected by Yuta Uehara
 * ver3.0 2015/ n/ n digitalInterrupt
 * ver2.0 2015/10/ 9 improve dependency.
 * ver1.0 2015/ 3/ 3 The first version.
 ******************************************/

class DigitalInterruptHandler{
public:
	virtual void digitalInterrupt()=0;
};

class Digital{
protected:
	//***************override******************
	virtual int _digitalWrite()=0;
	virtual void _digitalWrite(int)=0;
	virtual int _digitalRead()=0;
	virtual int _setupDigitalIn()=0;
	virtual int _setupDigitalOut()=0;
	virtual int _setupDigitalInPullUp()=0;
	virtual int _setupDigitalInPullDown()=0;
	virtual int _setupDigitalInterrupt(int interruptMode){return 1;};
	//***************override******************

	enum{
		DIGITAL_MODE_NULL,
		DIGITAL_MODE_IN,
		DIGITAL_MODE_OUT,
		DIGITAL_MODE_PULLUP,
		DIGITAL_MODE_PULLDOWN,
	};
	int _digitalMode;
public:
	virtual int setupDigitalIn();
	virtual int setupDigitalOut();
	virtual int setupDigitalInPullUp();
	virtual int setupDigitalInPullDown();
	virtual int setupDigitalInterrupt(int interruptMode);
	virtual int addInterruptHandler(DigitalInterruptHandler &handler){return 1;};
	virtual void stopInterrupt(){};

	virtual void digitalWrite(int value);
	virtual int digitalRead();
	virtual void digitalHigh();
	virtual void digitalLow();
	virtual void digitalToggle();

	enum{
		INTERRUPT_NULL,
		INTERRUPT_RISING,
		INTERRUPT_FALLING,
		INTERRUPT_RISING_FALLING
	};

	Digital& operator= (int value) {
		digitalWrite(value);
		return *this;
	}

	Digital& operator= (Digital& dig) {
		digitalWrite(dig.digitalRead());
		return *this;
	}

	operator int() {
		return digitalRead();
	}

};

class DigitalInterruptHandlerNull:public DigitalInterruptHandler{
public:
	virtual void digitalInterrupt()=0;
};
#include <stdio.h>
class DigitalNull:public Digital{
public:
	DigitalNull(bool printfAvailable=true){
		_setupDigitalInterruptReturnValue=0;
		_setupDigitalReturnValue=0;
		this->printfAvailable=printfAvailable;
	}

	virtual int _digitalWrite(){return value;};
	virtual void _digitalWrite(int arg){value=arg;};
	virtual int _digitalRead(){return value;};
	virtual int _setupDigitalIn(){if(printfAvailable){printf("DigitalNull::setup\n");}return _setupDigitalReturnValue;};
	virtual int _setupDigitalOut(){if(printfAvailable){printf("DigitalNull::setup\n");}return _setupDigitalReturnValue;};
	virtual int _setupDigitalInPullUp(){if(printfAvailable){printf("DigitalNull::setup\n");}return _setupDigitalReturnValue;};
	virtual int _setupDigitalInPullDown(){if(printfAvailable){printf("DigitalNull::setup\n");}return _setupDigitalReturnValue;};
	virtual int _setupDigitalInterrupt(int interruptMode){return _setupDigitalInterruptReturnValue;};
protected:
	int _setupDigitalInterruptReturnValue;
	int _setupDigitalReturnValue;
	bool printfAvailable;

	int value;
};
