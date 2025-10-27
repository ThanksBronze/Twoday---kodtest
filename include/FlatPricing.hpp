#pragma once

#include <stdexcept>
#include <chrono>
#include "Pricing.hpp"

// Simple flat pricing model using hourly and daily rates.
class FlatPricing: public Pricing{
	public:
		FlatPricing(long hourly, long daily) : hourly(hourly), daily(daily) {}

		/**
		 * Calculates the price for a parking session.
		 * Returns price by  hourly or daily rate depending on duration.
		 * Rounds up to the next full rate.
		 */
		Currency calculatePrice(const Instant start, const Instant end) const override;

	private:
		const long hourly;
		const long daily;
};