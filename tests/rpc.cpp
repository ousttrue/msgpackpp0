#include <msgpackpp.h>
#include <catch.hpp>


TEST_CASE("procedure_call")
{
	// 0 args
	REQUIRE(1 == msgpackpp::procedure_call([]{ return 1; }));
	// 2 args
	REQUIRE(3==msgpackpp::procedure_call([](int a, int b) { return a + b; }, 1, 2));
	REQUIRE(-1== msgpackpp::procedure_call([](int a, int b) { return a - b; }, 1, 2));
}
