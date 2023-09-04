#pragma once

/*******************************************
 * encoder ver3.0 2022/01/31
 * Encoder class.
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Change history]
 * ver3.0 2022/01/31 new Encoder class.
 * ver2.0 2015/10/ 9 improve dependency.
 * ver1.0 2015/ 3/ 3 The first version.
 ******************************************/

namespace encoder {

class Encoder {
public:
	Encoder() = default;
	virtual ~Encoder() = default;

	virtual int setup() = 0;
	virtual void cycle() = 0;
	virtual void rev(bool rev) = 0;
	virtual float radian() const = 0;
};

class EncoderCount: public Encoder {
public:
	EncoderCount() = default;
	virtual ~EncoderCount() = default;

	virtual void cpr(int cpr) = 0;
	virtual int count() const = 0;
};

}