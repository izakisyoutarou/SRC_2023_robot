#include "cycle_once_checker.hpp"

namespace cycle_once_checker {
	
int64_t CycleCounter::count_ = 0;

bool CycleOnceChecker::check() {
	const auto count = CycleCounter::count();
	if(count > old_count || count == 0) {
		old_count = count;
		return true;
	}
	else {
		return false;
	}
}

}