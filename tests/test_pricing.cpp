#include <catch2/catch.hpp>
#include <chrono>
#include <stdexcept>

#include "FlatPricing.hpp"
#include "Models.hpp"
#include "TestClock.hpp"

struct test_pricing{
	std::int64_t mins;
	std::int64_t expected_price;
	const char* case_name;
};

/*
	Tests for calculatePrice()
==================================
*/

TEST_CASE("calculatePrice() - Time under 24 hours uses 15 sek per hour."){
	FlatPricing pricing(15, 50);
	
	test_pricing cases[] = {
		{0, 0, "0 minute should result in 0 SEK"},
		{1, 15, "1 minute should result in 15 SEK"},
		{30, 15, "30 minutes should be 15 SEK"},
		{60, 15, "60 minutes should be 15 SEK"},
		{ 61, 30, "61 minutes should be 30 SEK" },
		{23 * 60 + 59, 360, "23h and 59 minutes should be 360 SEK"}
	};

	for (const test_pricing& c : cases){
		SECTION(c.case_name){
			Instant start = std::chrono::system_clock::now();
			TestClock clock(start);

			clock.advanceMinutes(c.mins);

			Currency price = pricing.calculatePrice(start, clock.now());
			
			REQUIRE(price.toSEK() == c.expected_price);
		}
	}
}

TEST_CASE("calculatePrice() - Time 24 hours and up uses fixed rate"){
	FlatPricing pricing(15, 50);

	test_pricing cases[]{
		{24 * 60, 50, "24h should result in 50 SEK"},
		{24 * 60 + 1, 100, "24h and 1 minute should result in 100 SEK"},
		{48 * 60, 100, "48h should result in 100 SEK"},
		{48 * 60 + 1, 150, "48h and 1 minute should result in 150 SEK"},
		{7 * 24 * 60 + 1, 400, "7 days and 1 minute should result in 400 SEK"},
	};

	for (const test_pricing& c : cases){
		SECTION(c.case_name){
			Instant start = std::chrono::system_clock::now();
			TestClock clock(start);

			clock.advanceMinutes(c.mins);

			Currency price = pricing.calculatePrice(start, clock.now());

			REQUIRE(price.toSEK() == c.expected_price);
		}
	}
}

TEST_CASE("calculatePrice() - Start time > end time"){
	FlatPricing pricing(15, 50);
	Instant start = std::chrono::system_clock::now();
	TestClock clock(start);

	clock.advanceMinutes(-30);

	REQUIRE_THROWS_AS(pricing.calculatePrice(start, clock.now()), std::invalid_argument);
}