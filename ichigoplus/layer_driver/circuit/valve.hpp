#pragma once

class Valve{
protected:
	bool turnOverValue;
public:
	virtual int setup(bool value=false)=0;
	virtual void on()=0;
	virtual void off()=0;
	virtual void toggle()=0;
	virtual int read()=0;
	void turnOver(bool value){turnOverValue=value;};
};
