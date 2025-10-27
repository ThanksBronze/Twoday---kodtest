#pragma once

#include "Clock.hpp"

//Clock used for tests where time is controlled manually.
class TestClock: public Clock{
	public:
		explicit TestClock(Instant start) : current(start) {}

		// Returns the currently stored time.
		Instant now() const override;

		// Adjusts the time with given number of minutes.
		void advanceMinutes(int64_t mins);

	private:
		Instant current;
};