add_library(parking_core
	FlatPricing.cpp
	InMemoryParkingDataHandler.cpp
	ParkingLot.cpp
	ParkingService.cpp
	SystemClock.cpp
	TestClock.cpp
)

target_include_directories(parking_core
	PUBLIC
		${CMAKE_SOURCE_DIR}/include
)