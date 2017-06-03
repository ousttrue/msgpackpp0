#include <msgpackpp.h>
//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>
#include <tuple>


template<typename T>
void Test(T src)
{
	// serialize
	msgpackpp::packer packer;
	packer << src;
	auto p = packer.get_payload();

	// deserialize
	auto parsed = msgpackpp::parser(p.data(), p.size());
	T value;
	parsed >> value;

	REQUIRE(src == value);
}


TEST_CASE("serialize")
{
	Test(true);
	Test(false);
	Test(0);
	Test(1);
	Test(std::numeric_limits<std::uint8_t>::max());
	Test(std::numeric_limits<std::uint8_t>::max()+1);
	Test(std::numeric_limits<std::uint16_t>::max()+1);
	Test(std::numeric_limits<std::uint32_t>::max()+1);
	Test(-32);
	Test(std::numeric_limits<std::int8_t>::min());
	Test(std::numeric_limits<std::int8_t>::min() - 1);
	Test(std::numeric_limits<std::int16_t>::min() - 1);
	Test(std::numeric_limits<std::int32_t>::min() - 1);
	Test(1.1f);
	Test(1.1);

	Test(std::vector<std::uint8_t>{1, 2, 3});

	Test(std::string(u8"���{��"));

	Test(std::make_tuple(true, 0, 1.5, std::string(u8"ABC")));
}