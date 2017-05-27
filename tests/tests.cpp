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
	auto p = msgpackpp::packer().pack_nil().get_payload();

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
	auto p = msgpackpp::packer().pack_bool(false).get_payload();

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
	auto p = msgpackpp::packer().pack_bool(true).get_payload();

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
	auto p = msgpackpp::packer().pack_integer(1).get_payload();

	// check
	REQUIRE(1 == p.size());
	REQUIRE(0x01 == p[0]);

	// unpack
	auto parsed = msgpackpp::parser(p.data(), p.size());
	REQUIRE(1 == parsed.get_integer<std::uint8_t>());
}

/// negative fixnum stores 5-bit negative integer(-1 to -32)
/// +--------+
/// |111YYYYY|
/// +--------+
TEST_CASE("small_negative_int")
{
	// packing
	auto p = msgpackpp::packer().pack_integer(-1).get_payload();

	// check
	REQUIRE(1 == p.size());
	REQUIRE(msgpackpp::pack_type::NEGATIVE_FIXNUM_0x01 == p[0]);

	// unpack
	auto parsed = msgpackpp::parser(p.data(), p.size());
	REQUIRE(-1 == parsed.get_integer<std::int8_t>());
}

/// uint 8 stores a 8-bit unsigned integer
/// +--------+--------+
/// |  0xcc  |ZZZZZZZZ|
/// +--------+--------+
TEST_CASE("uint8")
{
	// packing
	auto p = msgpackpp::packer().pack_integer(128).get_payload();

	// check
	REQUIRE(2 == p.size());
	REQUIRE(0xcc == p[0]);

	// unpack
	auto parsed = msgpackpp::parser(p.data(), p.size());
	REQUIRE(128 == parsed.get_integer<std::uint8_t>());
}

/// uint 16 stores a 16-bit big-endian unsigned integer
/// +--------+--------+--------+
/// |  0xcd  |ZZZZZZZZ|ZZZZZZZZ|
/// +--------+--------+--------+
TEST_CASE("uint16")
{
	// packing
	auto p = msgpackpp::packer().pack_integer(256).get_payload();

	// check
	REQUIRE(3 == p.size());
	REQUIRE(0xcd == p[0]);
	REQUIRE(0x01 == p[1]);
	REQUIRE(0x00 == p[2]);

	// unpack
	auto parsed = msgpackpp::parser(p.data(), p.size());
	REQUIRE(256 == parsed.get_integer<std::uint16_t>());
}

/// uint 32 stores a 32-bit big-endian unsigned integer
/// +--------+--------+--------+--------+--------+
/// |  0xce  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ
/// +--------+--------+--------+--------+--------+
TEST_CASE("uint32")
{
	{
		int value = 65536;

		// packing
		auto p = msgpackpp::packer().pack_integer(value).get_payload();

		// check
		REQUIRE(5==p.size());
		REQUIRE(0xce==p[0]);
		REQUIRE(0x00 == p[1]);
		REQUIRE(0x01 == p[2]);
		REQUIRE(0x00 == p[3]);
		REQUIRE(0x00 == p[4]);

		// unpack
		auto parsed = msgpackpp::parser(p.data(), p.size());
		REQUIRE(value==parsed.get_integer<std::uint32_t>());
	}

	{
		unsigned int value = 4294967295;

		// packing
		auto p = msgpackpp::packer().pack_integer(value).get_payload();

		// check
		REQUIRE(5 == p.size());
		REQUIRE(0xce == p[0]);

		// unpack
		auto parsed = msgpackpp::parser(p.data(), p.size());
		REQUIRE(value == parsed.get_integer<std::uint32_t>());
	}
}
