#include <stdexcept>

#include "InMemoryParkingDataHandler.hpp"


std::optional<ParkingTicket> InMemoryParkingDataHandler::getActiveTicket(SpotId spot) const{
	auto iterator = findEntry(spot);

	if (iterator != activeSpots.end()){
		return iterator->ticket;
	}else return std::nullopt;
}

void InMemoryParkingDataHandler::saveTicket(ParkingTicket ticket){
	if(isFree(ticket.getSpot())){
		activeSpots.push_back(Entry{ticket.getSpot(), std::move(ticket)});
	}else throw std::invalid_argument("Invalid spot: Spot is already occupied.");
}

void InMemoryParkingDataHandler::clearSpot(SpotId spot) {
	auto iterator = findEntry(spot);

	if (iterator != activeSpots.end()){
		activeSpots.erase(iterator);
	}else throw std::invalid_argument("Invalid spot: Nothing is parked on " + spot.toString() + ".");
}

bool InMemoryParkingDataHandler::isFree(SpotId spot) const{
	if(capacity.isValid(spot)){
		auto iterator = findEntry(spot);
		
		if (iterator != activeSpots.end()){
			return false;
		}else {return true;};
	}else throw std::invalid_argument("Invalid spot: Parkingspot " + spot.toString() + " does not exist.");
}

auto InMemoryParkingDataHandler::findEntry(SpotId spot) const -> It{
	return std::find_if(activeSpots.cbegin(), activeSpots.cend(), [&](const Entry& e){
		return (e.spot.getFloor() == spot.getFloor() && e.spot.getIndex() == spot.getIndex());
	});
}
