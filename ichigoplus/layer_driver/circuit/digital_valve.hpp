#pragma once
#include "valve.hpp"
#include "ichigoplus/layer_driver/base/digital.hpp"

class DigitalValve :public Valve{
public:
	Digital &pin;
	DigitalValve(Digital &digital):pin(digital){turnOverValue=false;};
	int setup(bool value=false){turnOverValue=value;return pin.setupDigitalOut();};
	void on(){pin.digitalWrite(!turnOverValue);};
	void off(){pin.digitalWrite(turnOverValue);};
	void toggle(){pin.digitalToggle();};
	int read(){return pin.digitalRead();};
};
