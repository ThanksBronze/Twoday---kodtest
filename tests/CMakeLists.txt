find_package(Catch2 2 REQUIRED)

set(TEST_SOURCES
	test_pricing.cpp
	test_dataHandler.cpp
	test_ParkingLot.cpp
	test_ParkingService.cpp
	main.cpp
)

add_executable(run_tests ${TEST_SOURCES})

target_link_libraries(run_tests
	PRIVATE
		parking_core
		Catch2::Catch2
)

target_include_directories(run_tests
	PRIVATE
		parking_core
)

enable_testing()
add_test(NAME run_tests COMMAND $<TARGET_FILE:run_tests>)