#pragma once
/*******************************************
 * cycle_once_checker 2021/09/15
 * This program determine if the cycle call is the first in the controle period.
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver1.0 2021/09/15 The first version
 ******************************************/
#include <stdint.h>

namespace cycle_once_checker {

class CycleCounter {
public:
	CycleCounter() = default;
	void cycle() { count_++; }
	void operator()() { cycle(); }
	static int64_t count() { return count_; }
private:
	static int64_t count_;
};

class CycleOnceChecker {
public:
	CycleOnceChecker() = default;
	bool check();
	bool operator()() { return check(); }
private:
	int64_t old_count = 0;
};

}