#include <msgpackpp.h>
#include <catch.hpp>
#include <tuple>
#include <functional>


namespace msgpackpp {
namespace rpc {

    typedef std::vector<std::uint8_t> bytes;
    typedef std::function<bytes(const bytes&)> procedurecall;

	
	template<typename F, typename R, typename C, typename ...AS, std::size_t... IS>
	procedurecall _make_procedurecall(const F &f
		, R(C::*)(AS...)const
		, std::index_sequence<IS...>
	)
	{
		// request -> response ‚Å‚Í‚È‚­params -> result
		return [f](const bytes& src)->bytes
		{
			// unpack args
			auto parser = msgpackpp::parser(src);
			std::tuple<AS...> args;
			parser >> args;

			// call
			auto r = f(std::get<IS>(args)...);

			// pack result
			msgpackpp::packer packer;
			packer << r;
			return packer.get_payload();
		};
	}

	template<typename F, typename R, typename C, typename ...AS>
	procedurecall _make_procedurecall(F f
		, R(C::*)(AS...)const
	)
	{
		return _make_procedurecall(f
			, &decltype(f)::operator()
			, std::index_sequence_for<AS...>{}
		);
	}

    template<typename F>
	procedurecall make_procedurecall(F f)
	{
		return _make_procedurecall(f
			, &decltype(f)::operator()
		);
	}
}
}

template<typename F, typename R, typename C, typename ...AS>
decltype(auto) _msgpack_call(F f, R(C::*)(AS...)const, AS... args)
{
	auto proc = msgpackpp::rpc::make_procedurecall(f);
	msgpackpp::packer packer;
	packer << std::make_tuple(args...);
	auto result = proc(packer.get_payload());

	R value;
	msgpackpp::parser(result) >> value;
	return value;
}

template<typename F, typename ...AS>
decltype(auto) msgpack_call(F f, AS... args)
{
	return _msgpack_call(f, &decltype(f)::operator(), args...);
}

TEST_CASE("tuple_call")
{
	REQUIRE(3==msgpack_call([](int a, int b) { return a + b; }, 1, 2));
	REQUIRE(-1==msgpack_call([](int a, int b) { return a - b; }, 1, 2));
}
