#pragma once

class MotorDriver{
public:
	virtual int setup()=0;
	virtual void cycle()=0;
	virtual void outRev(bool)=0;
	virtual void outRev()=0;
	virtual void duty(float value)=0;
	virtual float duty()=0;
	virtual void free()=0;
};
