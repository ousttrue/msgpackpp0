#include <msgpackpp.h>
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>


TEST_CASE("serialize int")
{
    // serialize
	auto packer = msgpackpp::packer();
    packer << 1;
	auto p = packer.get_payload();

	// deserialize
	auto parsed = msgpackpp::parser(p.data(), p.size());
    int value;
    parsed >> value;

	REQUIRE(1 == value);
}
