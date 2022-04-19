
#include "utilities.h"

#include <chrono>

unsigned long long utility::time::timeSinceEpoch() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}