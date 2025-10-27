#pragma once

#include "Clock.hpp"

// Real clock implementation using the system time.
class SystemClock: public Clock{
	public:
		// Returns the current time.
		Instant now() const override;
};