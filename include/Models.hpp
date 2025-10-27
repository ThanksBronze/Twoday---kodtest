/**
 * File containg basic data models used across the parking system.
 * Simple value objects without business logic.
 */
#pragma once

#include <chrono>
#include <string>

using Instant = std::chrono::system_clock::time_point;

/**
 * Represents a monetary value in SEK (integer only in current implementation).
 * No rounding or conversion logic included.
 */
class Currency{
	public:
		Currency(int64_t price) : priceSEK(price) {}

		int64_t toSEK() const {return priceSEK;}
	private:
		int64_t priceSEK;
};

/**
 * Identifies a parking spot by floor and index.
 * Range/indexing validation is handled by caller.
 */
class SpotId{

	public:
		SpotId(int64_t floor, int64_t index) : floor(floor), index(index) {}

		std::string toString() const {
			return "floor: " + std::to_string(floor) + " and spot: " + std::to_string(index);
		}

		int64_t getFloor() const{
			return floor;
		}

		int64_t getIndex() const{
			return index;
		}

	private:
		int64_t floor;
		int64_t index;
};

/**
 * Represents an active parking session.
 * End time and parking cost are calculated by caller.
 */
class ParkingTicket{
	public:
		ParkingTicket(SpotId spot, std::string registration, Instant start) :
		spot(spot), registration(registration), startTime(start) {}

		SpotId getSpot() {return spot;}

		std::string getRegistration() const {return registration;}

		Instant getStartTime() const {return startTime;}
	
	private:
		SpotId spot;
		std::string registration;
		Instant startTime;
};

/**
 * Represents a completed parking receipt with starttime, end time and price.
 * Created after parking is completed and parking cost calculated.
 */
class Receipt{
	public:
		Receipt(SpotId spot, std::string registration, Instant start, Instant end, Currency price) :
			spot(spot), registration(std::move(registration)), startTime(start), endTime(end), price(price) {}

		SpotId getSpot() const {return spot;}

		const std::string& getRegistration() const noexcept { return registration; }

		Instant getStartTime() const {return startTime;}

		Instant getEndTime() const {return endTime;}

		Currency getPrice() const {return price;};

	private:
		SpotId spot;
		std::string registration;
		Instant startTime;
		Instant endTime;
		Currency price;
};