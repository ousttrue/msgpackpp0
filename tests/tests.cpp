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

/// fixstr stores a byte array whose length is upto 31 bytes:
/// +--------+========+
/// |101XXXXX|  data  |
/// +--------+========+
TEST_CASE("fixstr")
{
	auto str = "abc";

	// packing
	auto p = msgpackpp::packer().pack_str(str).get_payload();

	// check
	REQUIRE((1 + 3) == p.size());

	// unpack
	auto parsed = msgpackpp::parser(p.data(), p.size());
	REQUIRE(parsed.get_string()==str);
}

/// str 8 stores a byte array whose length is upto (2^8)-1 bytes:
/// +--------+--------+========+
/// |  0xd9  |YYYYYYYY|  data  |
/// +--------+--------+========+
TEST_CASE("str8")
{
	auto str =
		"0123456789"
		"0123456789"
		"0123456789"
		"01"
		;

	// packing
	auto p = msgpackpp::packer().pack_str(str).get_payload();

	// check
	REQUIRE((2 + 32)==p.size());
	REQUIRE(0xd9==p[0]);

	// unpack
	auto parsed = msgpackpp::parser(p.data(), p.size());
	auto parsed_str = parsed.get_string();
	REQUIRE(parsed_str == str);
}

/// str 16 stores a byte array whose length is upto (2^16)-1 bytes:
/// +--------+--------+--------+========+
/// |  0xda  |ZZZZZZZZ|ZZZZZZZZ|  data  |
/// +--------+--------+--------+========+
TEST_CASE("str16")
{
	auto str =
		"0123456789" "0123456789" "0123456789" "0123456789" "0123456789" "0123456789"
		"0123456789" "0123456789" "0123456789" "0123456789" "0123456789" "0123456789"
		"0123456789" "0123456789" "0123456789" "0123456789" "0123456789" "0123456789"
		"0123456789" "0123456789" "0123456789" "0123456789" "0123456789" "0123456789"
		"0123456789" "0123456"
		;

	// packing
	auto p = msgpackpp::packer().pack_str(str).get_payload();

	// check
	REQUIRE((3 + 257)==p.size());
	REQUIRE(0xda==p[0]);

	// unpack
	auto parsed = msgpackpp::parser(p.data(), p.size());
	REQUIRE(parsed.get_string() == str);
}

/// str 32 stores a byte array whose length is upto (2^32)-1 bytes:
/// +--------+--------+--------+--------+--------+========+
/// |  0xdb  |AAAAAAAA|AAAAAAAA|AAAAAAAA|AAAAAAAA|  data  |
/// +--------+--------+--------+--------+--------+========+
TEST_CASE("str32")
{
	auto len = (0xFFFF + 2);
	char buf[0xFFFF + 2] = {};
	for (int i = 0; i<len; ++i)
	{
		buf[i]=('0' + (i % 10));
	}

	// packing
	auto p = msgpackpp::packer().pack_str(std::string(buf, buf+len)).get_payload();

	// check
	REQUIRE(0xdb == p[0]);
	REQUIRE((5 + len) == p.size());

	// unpack
	auto parsed = msgpackpp::parser(p.data(), p.size());
	REQUIRE(parsed.get_string() == std::string(buf, buf + len));
}

/// bin 8 stores a byte array whose length is upto (2^8)-1 bytes:
/// +--------+--------+========+
/// |  0xc4  |XXXXXXXX|  data  |
/// +--------+--------+========+
TEST_CASE("bin8")
{
	std::vector<unsigned char> buf;
	buf.push_back(0);

	// packing
	auto p = msgpackpp::packer().pack_bin(buf).get_payload();

	// check
	REQUIRE(2 + 1 == p.size());
	REQUIRE(0xc4 == p[0]);

	// unpack
	auto parsed = msgpackpp::parser(p.data(), p.size());
	REQUIRE(buf == parsed.get_binary());
}

/// bin 16 stores a byte array whose length is upto (2^16)-1 bytes:
/// +--------+--------+--------+========+
/// |  0xc5  |YYYYYYYY|YYYYYYYY|  data  |
/// +--------+--------+--------+========+
TEST_CASE("bin16")
{
	std::vector<unsigned char> buf;
	for (int i = 0; i<0xFF + 1; ++i) {
		buf.push_back(i % 0xFF);
	}

	// packing
	auto p = msgpackpp::packer().pack_bin(buf).get_payload();

	// check
	REQUIRE(3 + 0xFF + 1 == p.size());
	REQUIRE(0xc5 == p[0]);

	// unpack
	auto parsed = msgpackpp::parser(p.data(), p.size());
	REQUIRE(buf == parsed.get_binary());
}

/// bin 32 stores a byte array whose length is upto (2^32)-1 bytes:
/// +--------+--------+--------+--------+--------+========+
/// |  0xc6  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|  data  |
/// +--------+--------+--------+--------+--------+========+
TEST_CASE("bin32")
{
	std::vector<unsigned char> buf;
	for (int i = 0; i<0xFFFF + 1; ++i) {
		buf.push_back(i % 0xFF);
	}

	// packing
	auto p = msgpackpp::packer().pack_bin(buf).get_payload();

	// check
	REQUIRE(5 + 0xFFFF + 1 == p.size());
	REQUIRE(0xc6 == p[0]);

	// unpack
	auto parsed = msgpackpp::parser(p.data(), p.size());
	REQUIRE(buf == parsed.get_binary());
}
