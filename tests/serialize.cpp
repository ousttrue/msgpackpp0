#include <msgpackpp.h>
//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>


template<typename T>
void Test(T src)
{
	// serialize
	auto packer = msgpackpp::packer();
	packer << src;
	auto p = packer.get_payload();

	// deserialize
	auto parsed = msgpackpp::parser(p.data(), p.size());
	T value;
	parsed >> value;

	REQUIRE(src == value);
}


TEST_CASE("serialize true")
{
	Test(true);
	Test(false);
}

TEST_CASE("serialize int")
{
	Test(1);
}
