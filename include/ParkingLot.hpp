#pragma once

#include "Models.hpp"

/**
 * Simple model describing parking lot capacity.
 * Contains number of total spots (no per-zone logic).
 */
class ParkingLot{
	public:
		ParkingLot(int64_t floors, int64_t spots) : floors(floors), spots(spots){}

		// Validates if a given spot is within parking lot dimensions.
		bool isValid(SpotId spot) const;

		// Returns total number of spots in the lot.
		int64_t totalSpots() const;
	
	private:
		int64_t floors;
		int64_t spots;
};