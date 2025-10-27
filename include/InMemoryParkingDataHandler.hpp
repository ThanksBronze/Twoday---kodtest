#pragma once

#include <vector>
#include <algorithm>

#include "ParkingDataHandler.hpp"
#include "ParkingLot.hpp"

// Simple in memory data handler (stored in the heap).
class InMemoryParkingDataHandler: public ParkingDataHandler{
	public:
		InMemoryParkingDataHandler(ParkingLot capacity): capacity(capacity), activeSpots(){}

		/**
		 * Returns the ticket containg the given spot if it exists,
		 * else returns std::nullopt.
		 * */
		std::optional<ParkingTicket> getActiveTicket(SpotId spot) const override;

		// Stores the ticket in the ticket-vector.
		void saveTicket(ParkingTicket ticket) override;

		// Removes the ticket from the ticket-vector.
		void clearSpot(SpotId spot) override;
		
		/*
		 * Returns true if a ticket matching the given spot exists,
		 * else false.
		 * */
		bool isFree(SpotId spot) const override;

	private:
		struct Entry{
			SpotId spot;
			ParkingTicket ticket;
		};
		
		ParkingLot capacity;
		std::vector<Entry> activeSpots;
		using It = std::vector<Entry>::const_iterator;
		
		/**
		 * Helper function to iterate over vector to find ticket
		 * by matching spot.
		 */
		auto findEntry(SpotId spot) const -> It;
};