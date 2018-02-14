if (UNIX)
	find_package(benchmark CONFIG REQUIRED)
	find_package(GTest CONFIG REQUIRED)
	find_package(GMock CONFIG REQUIRED)

	# Set for each "series" of test
	set(TEST_PREFIX common)
	set(ZIA_LIB zia_common)

	# Benchmark
	file(GLOB_RECURSE ALL_BENCH_CPP bench/*.cpp)

	foreach(ONE_BENCH_CPP ${ALL_BENCH_CPP})

		get_filename_component(ONE_BENCH_EXEC ${ONE_BENCH_CPP} NAME_WE)

		# Avoid name collision 
		set(TARGET_NAME Bench_${TEST_PREFIX}_${ONE_BENCH_EXEC})

		add_executable(${TARGET_NAME} ${ONE_BENCH_CPP})
		target_link_libraries(${TARGET_NAME} ${ZIA_LIB} benchmark::benchmark)
		add_sanitizers(${TARGET_NAME})

		# If you want to run benchmarks with the "make test" command, uncomment me
		add_test(${TARGET_NAME} ${TARGET_NAME})
	endforeach()

	# Unit tests
	file(GLOB_RECURSE ALL_UNIT_CPP unit/*.cpp)

	foreach(ONE_UNIT_CPP ${ALL_UNIT_CPP})

	get_filename_component(ONE_UNIT_EXEC ${ONE_UNIT_CPP} NAME_WE)

	# Avoid name collision 
	set(TARGET_NAME Unit_${TEST_PREFIX}_${ONE_UNIT_EXEC})

	add_executable(${TARGET_NAME} ${ONE_UNIT_CPP})
	target_link_libraries(${TARGET_NAME} ${ZIA_LIB} GMock::main GTest::main)
	add_sanitizers(${TARGET_NAME})

	# If you want to run benchmarks with the "make test" command, uncomment me
	add_test(${TARGET_NAME} ${TARGET_NAME})
	endforeach()
endif()