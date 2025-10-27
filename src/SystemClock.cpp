#include "SystemClock.hpp"

Instant SystemClock::now() const{
	return std::chrono::system_clock::now();
}