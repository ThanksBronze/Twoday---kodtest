#include "TestClock.hpp"

Instant TestClock::now() const{
	return current;
}

void TestClock::advanceMinutes(int64_t mins){
	current += std::chrono::minutes(mins);
}