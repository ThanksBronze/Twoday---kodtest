#pragma once

#include <optional>

#include "Models.hpp"

// Interface for storing and retrieving active parking tickets.
class ParkingDataHandler{
	public:
		virtual ~ParkingDataHandler() = default;

		/**
		 * Returns the ticket containg the given spot if it exists.
		 * Implementations decide how non existing ticket is handeld.
		 */
		virtual std::optional<ParkingTicket> getActiveTicket(SpotId spot) const = 0;

		/**
		 * Stores the given ticket.
		 * Implementations decide how ticket is stored.
		 */
		virtual void saveTicket(ParkingTicket ticket) = 0;

		// Removes the ticket matching the given spot.
		virtual void clearSpot(SpotId spot) = 0;

		// Returns true if a ticket matching the given spot exists, else false.
		virtual bool isFree(SpotId spot) const = 0;
};