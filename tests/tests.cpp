#include <msgpackpp.h>
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>


/// nil:
/// +--------+
/// |  0xc0  |
/// +--------+
TEST_CASE("nil") 
{
    // packing
    auto packer=msgpackpp::create_packer();
    packer.pack_nil();

    auto p=packer.get_payload();

    // check
    REQUIRE(1 == p.size());
    REQUIRE(0xc0 == p[0]);
}

/// false:
/// +--------+
/// |  0xc2  |
/// +--------+
TEST_CASE("false")
{
	// packing
	auto packer = msgpackpp::create_packer();
	packer.pack(false);

    auto p=packer.get_payload();

	// check
	REQUIRE(1 == p.size());
	REQUIRE(0xc2 == p[0]);
}

/// true:
/// +--------+
/// |  0xc3  |
/// +--------+
TEST_CASE("true")
{
	// packing
	auto packer = msgpackpp::create_packer();
	packer.pack(true);

    auto p=packer.get_payload();

	// check
	REQUIRE(1 == p.size());
	REQUIRE(0xc3 == p[0]);
}
