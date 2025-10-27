#include <catch2/catch.hpp>

#include "Models.hpp"
#include "ParkingLot.hpp"
#include "InMemoryParkingDataHandler.hpp"
#include "Clock.hpp"

/*
	Test for saveTicket() & getgetActiveTicket()
====================================================
*/
TEST_CASE("A occupied spot has a active ticket"){
	ParkingLot config(1, 5);
	std::unique_ptr<ParkingDataHandler> handler = std::make_unique<InMemoryParkingDataHandler>(config);
	SpotId spot(1, 2);
	Instant start = std::chrono::system_clock::now();
	ParkingTicket ticket(spot, "ABC012", start);

	handler->saveTicket(ticket);

	auto active(handler->getActiveTicket(spot));
	REQUIRE(active.has_value());
	

	REQUIRE(spot.getFloor() == active->getSpot().getFloor());
	REQUIRE(spot.getIndex() == active->getSpot().getIndex());
	REQUIRE(active->getRegistration() == "ABC012");
}

/*
	Test for saveTicket()
=============================
*/
TEST_CASE("saveTicket() - Saving a ticket makes the spot occupied"){
	ParkingLot config(1, 5);
	std::unique_ptr<ParkingDataHandler> handler = std::make_unique<InMemoryParkingDataHandler>(config);
	SpotId spot(1, 2);
	Instant start = std::chrono::system_clock::now();
	ParkingTicket ticket(spot, "ABC012", start);

	handler->saveTicket(ticket);

	REQUIRE_FALSE(handler->isFree(spot));
};

TEST_CASE("saveTicket() - throws on invalid spot"){
	ParkingLot config(3, 15);
	auto handler = std::make_unique<InMemoryParkingDataHandler>(config);
	SpotId bad(0, 0);
	Instant start = std::chrono::system_clock::now();
	ParkingTicket t(bad, "ABC012", start);

	REQUIRE_THROWS_AS(handler->saveTicket(t), std::invalid_argument);
}

/*
	Test for isFree()
==========================
*/
TEST_CASE("isFree() - Newly created parking spot is free"){
	ParkingLot config = ParkingLot(1, 5);
	std::unique_ptr<ParkingDataHandler> handler = std::make_unique<InMemoryParkingDataHandler>(config);
	SpotId spot(1, 2);

	REQUIRE(handler->isFree(spot));
};

TEST_CASE("isFree() - Throws on invalid spot") {
	ParkingLot config(3, 15);
	auto handler = std::make_unique<InMemoryParkingDataHandler>(config);
	SpotId bad(4, 1);
	REQUIRE_THROWS_AS(handler->isFree(bad), std::invalid_argument);
}

/*
	Tests for clearSpot()
============================
*/
TEST_CASE("clearSpot() - A cleared spot becomes free"){
	ParkingLot config(1, 5);
	std::unique_ptr<ParkingDataHandler> handler = std::make_unique<InMemoryParkingDataHandler>(config);
	SpotId spot(1, 2);
	Instant start = std::chrono::system_clock::now();
	ParkingTicket ticket(spot, "ABC012", start);

	handler->saveTicket(ticket);

	REQUIRE_FALSE(handler->isFree(spot));

	handler->clearSpot(spot);

	REQUIRE(handler->isFree(spot));
}

TEST_CASE("clearSpot() - Throws when nothing is parked") {
	ParkingLot config(3, 15);
	auto handler = std::make_unique<InMemoryParkingDataHandler>(config);
	SpotId spot{1, 2};
	REQUIRE_THROWS_AS(handler->clearSpot(spot), std::invalid_argument);
}


