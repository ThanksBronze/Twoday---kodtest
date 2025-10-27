#pragma once

#include <chrono>

#include "Models.hpp"

/**
 * Interface representing a time source.
 * Implementations decide where 'now()' gets value from.
 */
class Clock{
	public:
		virtual Instant now() const = 0;
};