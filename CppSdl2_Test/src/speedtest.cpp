// main() provided by Catch in file main.cpp.
#include <catch2/catch.hpp>

TEST_CASE("benchmarked", "[!benchmark]") {

	const int ITERATIONS = 1000;

	BENCHMARK("test with no precalculation") {

	};

	BENCHMARK("test with precalculate") {
	};

	BENCHMARK("calculate long expression") {
	};
}
