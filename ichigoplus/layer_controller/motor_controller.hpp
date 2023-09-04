#pragma once

class MotorController{
public:
	virtual void duty(float duty){};
	virtual	float duty(){return 0;};
	virtual void outRev(bool value){};
	virtual void outRev(){};
	virtual void encRev(bool value){};
	virtual void encRev(){};

	virtual void setGearRatio(float value){};//reduction gear ratio
	virtual float getGearRatio(){return 1;};

	virtual int setup(){return 1;};
	virtual void cycle(){};
	virtual void stop(){};

	virtual void pos(float pos){};
	virtual void vel(float vel){};
	virtual float idealPos(){return 0;};
	virtual float idealVel(){return 0;};

	virtual void setPosGain(float value){};
	virtual void setVelGain(float value){};
	virtual void setAccGain(float value){};
	virtual float getPosGain(){return 0;};
	virtual float getVelGain(){return 0;};
	virtual float getAccGain(){return 0;};

	virtual void setWeakPosGain(float value){};
	virtual void setWeakVelGain(float value){};
	virtual void setWeakAccGain(float value){};
	virtual float getWeakPosGain(){return 0;};
	virtual float getWeakVelGain(){return 0;};
	virtual float getWeakAccGain(){return 0;};
	virtual void setWeakGainArea(float radian){};
	virtual float getWeakGainArea(){return 1;};

	virtual void setWeakDirecPosGain(float value){};
	virtual void setWeakDirecVelGain(float value){};
	virtual void setWeakDirecAccGain(float value){};

	virtual float getWeakDirecPosGain(){return 0;};
	virtual float getWeakDirecVelGain(){return 0;};
	virtual float getWeakDirecAccGain(){return 0;};
	virtual void setWeakGainDirec(float weakGainDirec){};


	virtual void setLimitVel(float value){};
	virtual void setLimitAcc(float value){};
	virtual void setStdAcc(float value){};
	virtual void setLimitCurrent(float value){};

	virtual void setLimitPosDiff(float value){};
	virtual void setLimitVelDiff(float value){};

	virtual void setDutyAreaCw(float cw){};
	virtual void setDutyAreaCcw(float ccw){};
	virtual void setPosAreaCw(float cw){};
	virtual void setPosAreaCcw(float ccw){};

	virtual int count(){return 0;}
	virtual float revolution(){return 0;}
	virtual float radian(){return 0;}
	virtual float rps(){return 0;}
	virtual float rads(){return 0;}

	virtual void setSourceVoltage(float value){};//[V]
	virtual void setSpeedConstant(float value){};//[V/rads]
	virtual void setForwardSpeedConstant(float value){};
	virtual void setBackSpeedConstant(float value){};
	virtual void setTorqueConstantData(float value){};//[rads]
	virtual void setTerminalResistance(float value){};//[ƒ¶]

	virtual void current(float value){};
};
