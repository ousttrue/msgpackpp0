#include <msgpackpp.h>
#include <catch.hpp>


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


struct Person
{
	std::string name;
	int age;

	bool operator==(const Person &rhs)const
	{
		return name == rhs.name && age == rhs.age;
	}
};
namespace msgpackpp
{
	// Person p;
	// msgpackpp::packer packer;
	// packer << p;
	void serialize(packer &packer, const Person &p)
	{
		packer.pack_map(2)
			<< "name" << p.name
			<< "age" << p.age
			;
	}

	//  auto parser=msgpackpp::parser(msgpack_bytes);
	//  Person p;
	//  parser >> p;
	parser deserialize(const parser &u, Person &p)
	{
		auto uu = u[0];
		auto count = u.count();
		for (int i = 0; i<count; ++i)
		{
			auto key = uu.get_string();
			uu = uu.next();

			if (key == "name") {
				uu >> p.name;
			}
			else if (key == "age") {
				uu >> p.age;
			}
			else {
				// unknown key
				assert(false);
			}
			uu = uu.next();
		}
		return uu;
	}
}


struct Point
{
	float x;
	float y;

	bool operator==(const Point &rhs)const
	{
		return x == rhs.x && y == rhs.y;
	}
};
namespace msgpackpp
{
	// Point p;
	// msgpackpp::packer packer;
	// packer << p;
	void serialize(packer &packer, const Point &p)
	{
		packer.pack_array(2)
			<< p.x
			<< p.y
			;
	}

	//  auto parser=msgpackpp::parser(msgpack_bytes);
	//  Point p;
	//  parser >> p;
	parser deserialize(const parser &u, Point &p)
	{
		assert(u.count() == 2);

		auto uu = u[0];

		uu >> p.x;
		uu = uu.next();

		uu >> p.y;
		uu = uu.next();

		return uu;
	}
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
    Test(std::vector<int>{1, 2, 3});
    Test(std::vector<std::string>{"a", "ab", "hoge"});

	Test(std::vector<std::uint8_t>{1, 2, 3});

	Test(std::string(u8"“ú–{Œê"));

	Test(std::make_tuple(true, 0, 1.5, std::string(u8"ABC")));

	// user type
	Test(Person{ "hoge", 100 });
	Test(Point{ 1.5f, 2.5f });
}
