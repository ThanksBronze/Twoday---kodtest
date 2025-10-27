#include <catch2/catch.hpp>
#include "Models.hpp"
#include "ParkingLot.hpp"

struct test_ParkingLot{
	int64_t floor;
	int64_t space;
	int64_t expected_spots;
};

/*
	Tests for isValid()
===========================
*/
TEST_CASE("isValid() - Valid parking spots returns true"){
	ParkingLot config = ParkingLot(3, 15);
	test_ParkingLot cases[] = {
		{1, 1, 0},
		{1, 7, 0},
		{1, 15, 0},
		{2, 1, 0},
		{2, 7, 0},
		{2, 15, 0},
		{3, 1, 0},
		{3, 7, 0},
		{3, 15, 0}
	};

	for(auto c : cases){
		SpotId spot = SpotId(c.floor, c.space);
		REQUIRE(config.isValid(spot));	
	}
}

TEST_CASE("isValid() - Invalid parking spots returns false"){
	ParkingLot config = ParkingLot(3, 15);
	test_ParkingLot cases[] = {
		{1, 16, 0},
		{1, 34, 0},
		{5, 15, 0},
		{7, 1, 0},
		{5, 34, 0},
		{17, 16, 0}
	};

	for(auto c : cases){
		SpotId spot = SpotId(c.floor, c.space);
		REQUIRE_FALSE(config.isValid(spot));
	}
}

/*
	Tests for totalSpots()
==============================
*/
TEST_CASE("totalSpots() - returns the total amount of spots"){

	test_ParkingLot cases[] = {
		{1, 16, 16},
		{1, 34, 34},
		{5, 15, 5*15},
		{7, 1, 7},
		{5, 34, 5*34},
		{17, 16, 17*16}
	};

	for(auto c : cases){
		ParkingLot config = ParkingLot(c.floor, c.space);
		REQUIRE(config.totalSpots() == c.expected_spots);
	}
}

