#pragma once

#include "Models.hpp"

// Interface for calculating parking prices based on start and exit times.
class Pricing{
	public:
		virtual ~Pricing() = default;

		// Implementations define fitting pricing logic.
		virtual Currency calculatePrice(const Instant start, const Instant end) const = 0;
};