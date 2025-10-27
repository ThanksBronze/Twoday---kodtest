#include <catch2/catch.hpp>

#include "ParkingDataHandler.hpp"
#include "ParkingService.hpp"
#include "TestClock.hpp"
#include "Models.hpp"
#include "FlatPricing.hpp"
#include "InMemoryParkingDataHandler.hpp"
#include "ParkingLot.hpp"

/*
	Tests for park()
========================
*/

TEST_CASE("park() - Happy path works as intended."){
	ParkingLot lot(3, 15);
	InMemoryParkingDataHandler handler(lot);

	Instant start = std::chrono::system_clock::now();
	TestClock clock(start);

	FlatPricing pricing(15, 50);

	ParkingService service(handler, pricing, clock);

	SpotId spot(3, 5);
	std::string reg = "ABC123";

	service.park(spot, reg);

	REQUIRE_FALSE(service.isFree(spot));

	auto ticket = handler.getActiveTicket(spot);

	REQUIRE(ticket.has_value());
	

	REQUIRE(spot.getFloor() == ticket->getSpot().getFloor());
	REQUIRE(spot.getIndex() == ticket->getSpot().getIndex());
	REQUIRE(ticket->getRegistration() == "ABC123");
	REQUIRE(ticket->getStartTime() == start);
}

TEST_CASE("park() - Error when spot is already occupied") {
	ParkingLot lot(3, 15);
	InMemoryParkingDataHandler handler(lot);

	Instant start = std::chrono::system_clock::now();
	TestClock clock(start);

	FlatPricing pricing(15, 50);

	ParkingService service(handler, pricing, clock);

	SpotId spot(1, 3);
	std::string reg = "ABC123";

	service.park(spot, reg);

	REQUIRE_THROWS_AS(service.park(spot, "DEF456"), std::invalid_argument);
}

TEST_CASE("park() - invalid spot should throw") {
	ParkingLot lot(3, 15);
	InMemoryParkingDataHandler handler(lot);
	Instant start = std::chrono::system_clock::now();
	TestClock clock(start);
	FlatPricing pricing(15, 50);
	ParkingService service(handler, pricing, clock);

	SECTION("floor out of range (negative)") {
		SpotId bad(-1, 1);
		REQUIRE_THROWS_AS(service.park(bad, "ABC123"), std::invalid_argument);
	}

	SECTION("floor out of range (>= floors)") {
		SpotId bad(4, 1);
		REQUIRE_THROWS_AS(service.park(bad, "ABC123"), std::invalid_argument);
	}

	SECTION("index out of range (negative)") {
		SpotId bad(1, -1);
		REQUIRE_THROWS_AS(service.park(bad, "ABC123"), std::invalid_argument);
	}

	SECTION("index out of range (>= spotsPerFloor)") {
		SpotId bad(1, 16);
		REQUIRE_THROWS_AS(service.park(bad, "ABC123"), std::invalid_argument);
	}
}

TEST_CASE("park() - Empty/invalid registration should throw") {
	ParkingLot lot(3, 15);
	InMemoryParkingDataHandler handler(lot);
	Instant start = std::chrono::system_clock::now();
	TestClock clock(start);
	FlatPricing pricing(15, 50);
	ParkingService service(handler, pricing, clock);

	const SpotId good(1, 5);

	SECTION("empty string") {
		REQUIRE_THROWS_AS(service.park(good, ""), std::invalid_argument);
	}

	SECTION("whitespace only") {
		REQUIRE_THROWS_AS(service.park(good, "   \t  "), std::invalid_argument);
	}
}

TEST_CASE("park() - registration is trimmed and stored correctly") {
	ParkingLot lot(3, 15);
	InMemoryParkingDataHandler handler(lot);
	Instant start = std::chrono::system_clock::now();
	TestClock clock(start);
	FlatPricing pricing(15, 50);
	ParkingService service(handler, pricing, clock);

	SpotId spot(2, 10);
	service.park(spot, "  ABC123  ");
	auto t = handler.getActiveTicket(spot);

	REQUIRE(t.has_value());
	REQUIRE(t->getRegistration() == "ABC123");
}

/*
	Tests for pickup()
========================
*/

TEST_CASE("pickup() - Happy path works as intended."){
	ParkingLot lot(3, 15);
	InMemoryParkingDataHandler handler(lot);
	Instant start = std::chrono::system_clock::now();
	TestClock clock(start);
	FlatPricing pricing(15, 50);
	ParkingService service(handler, pricing, clock);

	SpotId spot(3, 5);
	std::string reg = "ABC123";
	service.park(spot, reg);

	clock.advanceMinutes(2*60);

	Receipt rec = service.pickup(spot, reg);

	REQUIRE(service.isFree(spot));
	REQUIRE(spot.getFloor() == rec.getSpot().getFloor());
	REQUIRE(spot.getIndex() == rec.getSpot().getIndex());
	REQUIRE(reg == rec.getRegistration());
	REQUIRE(30 == rec.getPrice().toSEK());
}

TEST_CASE("pickup() - pricing uses daily rate after 24h (rounded up per day)") {
	ParkingLot lot(3, 15);
	InMemoryParkingDataHandler handler(lot);
	Instant start = std::chrono::system_clock::now();
	TestClock clock(start);
	FlatPricing pricing(15, 50);
	ParkingService service(handler, pricing, clock);

	SpotId spot(1, 1);
	std::string reg = "EFG456";
	service.park(spot, reg);

	clock.advanceMinutes(25 * 60);

	Receipt rec = service.pickup(spot, reg);

	REQUIRE(service.isFree(spot));
	REQUIRE(rec.getPrice().toSEK() == 100);
}

TEST_CASE("pickup() - Pickup from empty spot should throw."){
	ParkingLot lot(3, 15);
	InMemoryParkingDataHandler handler(lot);
	Instant start = std::chrono::system_clock::now();
	TestClock clock(start);
	FlatPricing pricing(15, 50);
	ParkingService service(handler, pricing, clock);

	SpotId spot(3, 5);
	std::string reg = "ABC123";

	REQUIRE_THROWS_AS(service.pickup(spot, reg), std::runtime_error);
}

TEST_CASE("pickup() - Throw when registration doesn match."){
	ParkingLot lot(3, 15);
	InMemoryParkingDataHandler handler(lot);
	Instant start = std::chrono::system_clock::now();
	TestClock clock(start);
	FlatPricing pricing(15, 50);
	ParkingService service(handler, pricing, clock);

	SpotId spot(3, 5);
	std::string reg = "ABC123";
	service.park(spot, reg);

	clock.advanceMinutes(2*60);

	REQUIRE_THROWS_AS(service.pickup(spot, "EFG456"), std::invalid_argument);
}

TEST_CASE("pickup() - End time < start time should throw."){
	ParkingLot lot(3, 15);
	InMemoryParkingDataHandler handler(lot);
	Instant start = std::chrono::system_clock::now();
	TestClock clock(start);
	FlatPricing pricing(15, 50);
	ParkingService service(handler, pricing, clock);

	SpotId spot(3, 5);
	std::string reg = "ABC123";
	service.park(spot, reg);

	clock.advanceMinutes(-2*60);

	REQUIRE_THROWS_AS(service.pickup(spot, reg), std::invalid_argument);
}