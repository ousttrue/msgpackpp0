#include <msgpackpp.h>
#include <catch.hpp>


TEST_CASE("procedure_call")
{
	REQUIRE(3==msgpackpp::procedure_call([](int a, int b) { return a + b; }, 1, 2));
	REQUIRE(-1== msgpackpp::procedure_call([](int a, int b) { return a - b; }, 1, 2));
}
