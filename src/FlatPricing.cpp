#include <stdexcept>

#include "FlatPricing.hpp"

Currency FlatPricing::calculatePrice(const Instant start, const Instant end) const {
	if(start > end){
		throw std::invalid_argument("Invalid time: Start time cannot be after end time");
	}

	constexpr std::int8_t minsPerHour = 60;
	constexpr std::int16_t minsPerDay = 24 * minsPerHour;
	std::int64_t rate;
	std::int64_t rateTime;

	auto time_parked = std::chrono::duration_cast<std::chrono::minutes>(end-start);

	if(time_parked.count() < minsPerDay){
		rate = hourly;
		rateTime = minsPerHour;
	}else{
		rate = daily;
		rateTime = minsPerDay;
	}

	// Rounds up to next full hour or day.
	return Currency((((time_parked.count() + rateTime-1) / rateTime) * rate));
}