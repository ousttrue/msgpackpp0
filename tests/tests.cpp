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
    auto p=msgpackpp::packer().pack_nil().get_payload();

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
    auto p = msgpackpp::packer().pack(false).get_payload();

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
    auto p = msgpackpp::packer().pack(true).get_payload();

    // check
    REQUIRE(1 == p.size());
    REQUIRE(0xc3 == p[0]);
}

/// positive fixnum stores 7-bit positive integer
/// +--------+
/// |0XXXXXXX|
/// +--------+
TEST_CASE("small_int")
{
    // packing
    auto p=msgpackpp::packer().pack(1).get_payload();

    // check
    REQUIRE(1 == p.size());
    REQUIRE(0x01 == p[0]);

    // unpack
    auto parsed = msgpackpp::parser(p.data(), p.size());
    REQUIRE(1 == parsed.get_int());
}

/// negative fixnum stores 5-bit negative integer(-1 to -32)
/// +--------+
/// |111YYYYY|
/// +--------+
TEST_CASE("small_negative_int")
{
	// packing
	auto p = msgpackpp::packer().pack(-1).get_payload();

	// check
	REQUIRE(1==p.size());
	REQUIRE(msgpackpp::pack_type::NEGATIVE_FIXNUM_0x01==p[0]);

	// unpack
	auto parsed = msgpackpp::parser(p.data(), p.size());
	REQUIRE(-1 == parsed.get_int());
}

/// uint 8 stores a 8-bit unsigned integer
/// +--------+--------+
/// |  0xcc  |ZZZZZZZZ|
/// +--------+--------+
TEST_CASE("uint8")
{
	// packing
	auto p = msgpackpp::packer().pack(128).get_payload();

	// check
	REQUIRE(2 == p.size());
	REQUIRE(0xcc == p[0]);

	// unpack
	auto parsed = msgpackpp::parser(p.data(), p.size());
	REQUIRE(128, parsed.get_int());
}
