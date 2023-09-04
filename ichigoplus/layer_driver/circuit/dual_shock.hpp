#pragma once
class DualShock{
public:
    //=== button
	//--- option
	virtual int home()=0;
	virtual int start()=0;
	virtual int select()=0;
    //--- arrow
	virtual float up()=0;
	virtual float right()=0;
	virtual float left()=0;
	virtual float down()=0;
    //--- side
	virtual float l1()=0;
	virtual float l2()=0;
	virtual float r1()=0;
	virtual float r2()=0;
    //--- simble
	virtual float triangle()=0;
	virtual float circle()=0;
	virtual float cross()=0;
	virtual float square()=0;
    //--- accel
	virtual float accelX()=0;
	virtual float accelY()=0;
    //--- stick
	virtual float lx()=0;
	virtual float ly()=0;
	virtual int l3()=0;
	virtual float rx()=0;
	virtual float ry()=0;
	virtual int r3()=0;

	virtual int setup()=0;
	virtual int disconnect()=0;
    virtual int lowBattery()=0;
//=== 誤動作防止に閾値を設定
	virtual void setStickTolerance(float toleranceArg)=0;

	virtual void setArrowKeyTolerance(float toleranceArg)=0;

	virtual void setSymbolKeyTolerance(float toleranceArg)=0;

	virtual void setSide1KeyTolerance(float toleranceArg)=0;
	virtual void setSide2KeyTolerance(float toleranceArg)=0;

	virtual void setOptionKeyTolerance(float toleranceArg)=0;

	virtual void setlxTolerance(float toleranceArg)=0;
	virtual void setlyTolerance(float toleranceArg)=0;
	virtual void setrxTolerance(float toleranceArg)=0;
	virtual void setryTolerance(float toleranceArg)=0;

	virtual void setUpTolerance(float toleranceArg)=0;
	virtual void setRightTolerance(float toleranceArg)=0;
	virtual void setDownTolerance(float toleranceArg)=0;
	virtual void setLeftTolerance(float toleranceArg)=0;

	virtual void setTriangleTolerance(float toleranceArg)=0;
	virtual void setCircleTolerance(float toleranceArg)=0;
	virtual void setCrossTolerance(float toleranceArg)=0;
	virtual void setSquareTolerance(float toleranceArg)=0;

	virtual void setR1Tolerance(float toleranceArg)=0;
	virtual void setR2Tolerance(float toleranceArg)=0;
	virtual void setL1Tolerance(float toleranceArg)=0;
	virtual void setL2Tolerance(float toleranceArg)=0;
};
