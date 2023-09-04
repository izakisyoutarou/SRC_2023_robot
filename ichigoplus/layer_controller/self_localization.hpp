#pragma once
/*******************************************
 * self_localization ver2.0
 * This program is base class for selflocalization.
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver2.0 2022/02/06 Merge RC21_robot_lib into ichigoplus.
 * ver1.0 2021/01/12 The first version
 ******************************************/

#include "ichigoplus/lib_src/two_vector.hpp"
#include "coord.hpp"

namespace self_localization {

class SelfLocalization {
protected:
	using Coord = coord::Coordf;
	using TwoVector = rp_lib::TwoVectorf;
public:
	SelfLocalization() {}
	virtual ~SelfLocalization() {}

	virtual void init(const Coord &coord, float ang) = 0;
	virtual int setup() = 0;
	virtual void caribrate() = 0;

	virtual void cycle() = 0;

	virtual const Coord& coord() const = 0;
	virtual float angle() const = 0;
	virtual const TwoVector& vel() const = 0;
	virtual float angVel() const = 0;
};

class SelfLocalizationDebug: public SelfLocalization {
public:
	SelfLocalizationDebug() = default;

	void init(const Coord &coord, float ang) override { coord_ = coord; ang_ = ang;}
	int setup() override { return 0; }
	void caribrate() override {}

	void cycle() override {}
	const Coord& coord() const override { return coord_; }
	void coord(const Coord& coord) { coord_ = coord; }
	float angle() const override { return ang_; }
	void angle(float angle) { ang_ = angle; }
	const TwoVector& vel() const override { return vel_; }
	void vel(const TwoVector &vel) { vel_ = vel; }
	float angVel() const override { return angVel_; }
	void angVel(float angVel) { angVel_ = angVel; }

private:
	Coord coord_;
	float ang_ = 0.f;
	TwoVector vel_;
	float angVel_ = 0.f;
};

}
