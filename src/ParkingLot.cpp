#include "ParkingLot.hpp"

bool ParkingLot::isValid(SpotId spot) const{
	if(spot.getFloor() <= floors && spot.getFloor() > 0 &&
	spot.getIndex() <= spots && spot.getIndex() > 0){
		return true;
	}else{
		return false;
	}
}

int64_t ParkingLot::totalSpots() const {
	return (floors * spots);
}