#pragma once
/*******************************************
 * photo_interrputer ver2.0
 * This is base class of photo interrupter
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver2.0 2022/02/07 Change namespace. Merge into ichigoplus.
 * ver1.0 2021/09/07 The first version
 ******************************************/

#include "ichigoplus/layer_application/timer.hpp"
#include "ichigoplus/layer_driver/base/digital.hpp"
#include "ichigoplus/layer_driver/circuit/can_digital.hpp"

namespace photo_interrputer {

class PhotoInterrputer {
public:
	PhotoInterrputer() = default;
	virtual ~PhotoInterrputer() = default;

	virtual int setup() = 0;
	virtual void startReading(int sampling_period_ms) = 0;
	virtual void stopReading() = 0;
	virtual bool read() = 0;

	bool operator()() { return read(); }
};

class PhotoInterrputerDigital: public PhotoInterrputer {
public:
	enum class SetupType { in, inPullUp, inPullDown };
	PhotoInterrputerDigital(Digital &digital, SetupType setupType): digital(digital), setupType(setupType), old_value(digital.digitalRead()) { stopReading(); }
	int setup() override {
		if(setupType == SetupType::in) return digital.setupDigitalIn();
		else if(setupType == SetupType::inPullUp) return digital.setupDigitalInPullUp();
		else return digital.setupDigitalInPullDown();
	}
	void startReading(int sampling_period_ms = 0) override { 
		timer(sampling_period_ms, true);
		old_value = digital.digitalRead();
	}
	void stopReading() override { timer(1000*60*60*24, true); }
	bool read() override {
		if(timer()) old_value = digital.digitalRead();
		return old_value;
	}
private:
	Digital &digital;
	SetupType setupType;
	Timer timer;
	bool old_value;
};

class PhotoInterrputerCanDigitalPinInterface: public PhotoInterrputer {
public:
	enum class SetupType { in, inPullUp, inPullDown };
	PhotoInterrputerCanDigitalPinInterface(CanDigital::PinInterface &pinInterface, SetupType setupType): pinInterface(pinInterface), setupType(setupType) {}
	int setup() override {
		if(setupType == SetupType::in) return pinInterface.setupDigitalIn();
		else if(setupType == SetupType::inPullUp) return pinInterface.setupDigitalInPullUp();
		else return pinInterface.setupDigitalInPullDown();
	}
	void startReading(int sampling_period_ms) override { pinInterface.startDigitalRead(sampling_period_ms); }
	void stopReading() override { pinInterface.stopDigitalRead(); }
	bool read() override { return pinInterface.digitalRead(); }
private:
	CanDigital::PinInterface &pinInterface;
	SetupType setupType;
};

}