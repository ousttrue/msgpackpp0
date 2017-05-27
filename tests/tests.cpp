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
	REQUIRE(1 == parsed.get_number<std::uint8_t>());
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
	REQUIRE(-1 == parsed.get_number<std::int8_t>());
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
	REQUIRE(128 == parsed.get_number<std::uint8_t>());
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
	REQUIRE(256 == parsed.get_number<std::uint16_t>());
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
		REQUIRE(5 == p.size());
		REQUIRE(0xce == p[0]);
		REQUIRE(0x00 == p[1]);
		REQUIRE(0x01 == p[2]);
		REQUIRE(0x00 == p[3]);
		REQUIRE(0x00 == p[4]);

		// unpack
		auto parsed = msgpackpp::parser(p.data(), p.size());
		REQUIRE(value == parsed.get_number<std::uint32_t>());
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
		REQUIRE(value == parsed.get_number<std::uint32_t>());
	}
}

/// uint 64 stores a 64-bit big-endian unsigned integer
/// +--------+--------+--------+--------+--------+--------+--------+--------+--------+
/// |  0xcf  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|
/// +--------+--------+--------+--------+--------+--------+--------+--------+--------+
TEST_CASE("uint64")
{
	unsigned long long value = 4294967296;

	// packing
	auto p = msgpackpp::packer().pack_integer(value).get_payload();

	// check
	REQUIRE(9 == p.size());
	REQUIRE(0xcf == p[0]);
	REQUIRE(0x00 == p[1]);
	REQUIRE(0x00 == p[2]);
	REQUIRE(0x00 == p[3]);
	REQUIRE(0x01 == p[4]);
	REQUIRE(0x00 == p[5]);
	REQUIRE(0x00 == p[6]);
	REQUIRE(0x00 == p[7]);
	REQUIRE(0x00 == p[8]);

	// unpack
	auto parsed = msgpackpp::parser(p.data(), p.size());
	REQUIRE(value == parsed.get_number<std::uint64_t>());
}

/// int 8 stores a 8-bit signed integer
/// +--------+--------+
/// |  0xd0  |ZZZZZZZZ|
/// +--------+--------+
TEST_CASE("int8")
{
	char value = -33;

	// packing
	auto p = msgpackpp::packer().pack_integer(value).get_payload();

	// check
	REQUIRE(2 == p.size());
	REQUIRE(0xd0 == p[0]);
	REQUIRE(0xdf == p[1]);

	// unpack
	auto parsed = msgpackpp::parser(p.data(), p.size());
	REQUIRE(value == parsed.get_number<std::int8_t>());
}

/// int 16 stores a 16-bit big-endian signed integer
/// +--------+--------+--------+
/// |  0xd1  |ZZZZZZZZ|ZZZZZZZZ|
/// +--------+--------+--------+
TEST_CASE("int16")
{
	int value = -256;

	// packing
	auto p = msgpackpp::packer().pack_integer(value).get_payload();

	// check
	REQUIRE(3 == p.size());
	REQUIRE(0xd1 == p[0]);

	// unpack
	auto parsed = msgpackpp::parser(p.data(), p.size());
	REQUIRE(value == parsed.get_number<std::int16_t>());
}

/// int 32 stores a 32-bit big-endian signed integer
/// +--------+--------+--------+--------+--------+
/// |  0xd2  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|
/// +--------+--------+--------+--------+--------+
TEST_CASE("int32")
{
	int value = -65535;

	// packing
	auto p = msgpackpp::packer().pack_integer(value).get_payload();

	// check
	REQUIRE(5 == p.size());
	REQUIRE(0xd2 == p[0]);

	// unpack
	auto parsed = msgpackpp::parser(p.data(), p.size());
	REQUIRE(value == parsed.get_number<std::int32_t>());
}

/// int 64 stores a 64-bit big-endian signed integer
/// +--------+--------+--------+--------+--------+--------+--------+--------+--------+
/// |  0xd3  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|
/// +--------+--------+--------+--------+--------+--------+--------+--------+--------+
TEST_CASE("int64")
{
	long long value = -4294967296;

	// packing
	auto p = msgpackpp::packer().pack_integer(value).get_payload();

	// check
	REQUIRE(9 == p.size());
	REQUIRE(0xd3 == p[0]);

	// unpack
	auto parsed = msgpackpp::parser(p.data(), p.size());
	REQUIRE(value == parsed.get_number<std::int64_t>());
}

/// float 32 stores a floating point number in IEEE 754 single precision floating point number format:
/// +--------+--------+--------+--------+--------+
/// |  0xca  |XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX
/// +--------+--------+--------+--------+--------+
TEST_CASE("float32")
{
	float value = 1.5f;

	// packing
	auto p = msgpackpp::packer().pack_float(value).get_payload();

	// check
	REQUIRE(5== p.size());
	REQUIRE(0xca== p[0]);

	// unpack
	auto parsed = msgpackpp::parser(p.data(), p.size());
	REQUIRE(value==parsed.get_number<float>());
}

/// float 64 stores a floating point number in IEEE 754 double precision floating point number format:
/// +--------+--------+--------+--------+--------+--------+--------+--------+--------+
/// |  0xcb  |YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|
/// +--------+--------+--------+--------+--------+--------+--------+--------+--------+
TEST_CASE("float64")
{
	double value = 1.111111111111111111111111111111111111111111111111;

	// packing
	auto p = msgpackpp::packer().pack_double(value).get_payload();

	// check
	REQUIRE(9== p.size());
	REQUIRE(0xcb== p[0]);

	// unpack
	auto parsed = msgpackpp::parser(p.data(), p.size());
	REQUIRE(value==parsed.get_number<double>());
}
