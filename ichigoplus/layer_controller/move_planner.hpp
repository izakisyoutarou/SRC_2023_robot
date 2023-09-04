#pragma once
/*******************************************
 * move_planner
 * This class is interface for moving
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver2.0 2922/02/07 Merge into ichigoplus. Change namespace.
 * ver1.0 2021/01/29 The first version
 ******************************************/

#include "ichigoplus/lib_src/two_vector.hpp"
#include "coord.hpp"

namespace move_planner {

class MovePlanner {
protected:
	using Coord = coord::Coordf;
	using TwoVector = rp_lib::TwoVectorf;
public:
	MovePlanner() = default;
	virtual ~MovePlanner() = default;

	virtual Coord pos() const = 0;
	virtual TwoVector vel() const = 0;
};

}