#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>


TEST_CASE("benchmarked", "[!benchmark]") {

	const int ITERATIONS = 1000;

	BENCHMARK("test with no precalculation") {

	};

	BENCHMARK("test with precalculate") {
	};

	BENCHMARK("calculate long expression") {
	};
}
