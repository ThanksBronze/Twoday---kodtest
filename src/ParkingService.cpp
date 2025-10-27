#include "ParkingService.hpp"

void ParkingService::park(SpotId spot, std::string reg){
	trimRegistration(reg);
	if(reg.empty()){
		throw std::invalid_argument("Invalid registration: blank or wrong format.");;
	}

	handler.saveTicket(ParkingTicket(spot, reg, clock.now()));
}

bool ParkingService::isFree(SpotId spot){
	return handler.isFree(spot);
}

void ParkingService::trimRegistration(std::string &reg){
	size_t first = 0;
	while (first < reg.size() && std::isspace(static_cast<unsigned char>(reg[first]))) ++first;
	size_t last = reg.size();
	while (last > first && std::isspace(static_cast<unsigned char>(reg[last-1]))) --last;
	reg.erase(last);
	reg.erase(0, first);
}

Receipt ParkingService::pickup(SpotId spot, std::string reg){
	auto ticket = handler.getActiveTicket(spot);
	if(ticket.has_value()){
		Instant end = clock.now();

		trimRegistration(reg);
		if(reg == ticket->getRegistration()){
			handler.clearSpot(spot);
			return Receipt (spot, std::move(reg), ticket->getStartTime(), end, pricing.calculatePrice(ticket->getStartTime(), end));
		}else throw std::invalid_argument("Invalid registration: does not match the vehicle parked.");
	}else throw std::runtime_error("Invalid spot: Spot on " + spot.toString() + " is not occupied.");
}
