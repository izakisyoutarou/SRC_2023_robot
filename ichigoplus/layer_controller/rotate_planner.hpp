#pragma once
/*******************************************
 * rotate_planner
 * This class is interface for rotating
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver2.0 2022/02/07 Merge into ichigoplus. Change namespace.
 * ver1.0 2021/01/29 The first version.
 ******************************************/

namespace rotate_planner {

class RotatePlanner {
public:
	RotatePlanner() = default;
	virtual ~RotatePlanner() = default;

	virtual float pos() const  = 0;
	virtual float vel() const = 0;
};

}