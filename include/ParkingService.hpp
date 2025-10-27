#pragma once

#include "ParkingDataHandler.hpp"
#include "TestClock.hpp"
#include "Models.hpp"
#include "FlatPricing.hpp"
#include "InMemoryParkingDataHandler.hpp"
#include "ParkingLot.hpp"

/**
 * Service coordinating the parking operations.
 * Uses abstractions for time, pricing, and data handling.
 */
class ParkingService{
public:
	ParkingService(ParkingDataHandler& handler, Pricing& pricing, Clock& clock) 
	: handler(handler), pricing(pricing), clock(clock){}

	// Validates registration, saves the current time and starts a parking session.
	void park(SpotId spot, std::string reg);

	// Returns true if no ticket exists for spot, else false.
	bool isFree(SpotId spot);

	// Removes whitespace from registration string.
	void trimRegistration(std::string& reg);

	// Ends a parking session, validates registration and returns a receipt.
	Receipt pickup(SpotId spot, std::string reg);

private: 
	ParkingDataHandler& handler;
	Pricing& pricing;
	Clock& clock;
};
