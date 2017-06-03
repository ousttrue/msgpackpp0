# msgpackpp
msgpack parser &amp; packer

- header only
- simple

# msgpack spec
- https://github.com/msgpack/msgpack/blob/master/spec.md

# unittest framework
- https://github.com/philsquared/Catch

# usage
samples/sample.cpp

```c++
#include <msgpackpp.h>
#include <iostream>

int main(int argc, char **argv)
{
    // pack
    auto packer = msgpackpp::packer();
    packer.pack_array(4);
    packer.pack_integer(0);
    packer.pack_integer(256);
    packer.pack_str("method");
    packer.pack_map(3);
    packer.pack_str("x"); packer.pack_float(1.0f);
    packer.pack_str("y"); packer.pack_float(2.0f);
    packer.pack_str("z"); packer.pack_float(3.0f);

    // std::vector<std::uint8_t>
    auto p = packer.get_payload();

    // unpack
    auto u = msgpackpp::parser(p);
    std::cout << u << std::endl; // json style for debug

    std::cout << u.is_array() << std::endl;
    std::cout << u[0].get_number<int>() << std::endl;
    std::cout << u[1].get_number<int>() << std::endl;
    std::cout << u[2].get_string() << std::endl;
    std::cout << u[3].is_map() << std::endl;
    std::cout << u[3]["x"].get_number<float>() << std::endl;
    std::cout << u[3]["y"].get_number<float>() << std::endl;
    std::cout << u[3]["z"].get_number<float>() << std::endl;

    return 0;
}
```

```
[0,256,"method",{"x":1,"y":2,"z":3}]
1
0
256
method
1
1
2
3
```

see [tests](tests/tests.cpp).

# implemented types

| c type                   | packer                 | parser            |
| ------------------------ |:----------------------:|:-----------------:|
| (void)                   | packer.pack_nil()      | (parser.is_nil()) |
| bool                     | packer.pack_bool(is)   | parser.get_bool() |
| char                     | packer.pack_integer(n) | parser.get_number<char>() |
| short                    | packer.pack_integer(n) | parser.get_number<short>() |
| int                      | packer.pack_integer(n) | parser.get_number<int>() |
| long long                | packer.pack_integer(n) | parser.get_number<long long>() |
| unsigned char            | packer.pack_integer(n) | parser.get_number<unsigned char>() |
| unsigned short           | packer.pack_integer(n) | parser.get_number<unsigned short>() |
| unsigned int             | packer.pack_integer(n) | parser.get_number<unsigned int>() |
| unsigned long long       | packer.pack_integer(n) | parser.get_number<unsigned long long>() |
| float                    | packer.pack_float(n)   | parser.get_number<float>() |
| double                   | packer.pack_double(n)  | parser.get_number<double>() |
| const char *             | packer.pack_str(str)   |              |
| std::string              | packer.pack_str(str)   | parser.get_str() |
| std::vector<std::uint8_t>| packer.pack_bin(bin)   | parser.get_bin() |
| std::tuple<A...>         | packer << t(serializerr)| parser >> t(deserializer)|

# serializer

use ``operator<<``

## define serializer

```cpp
namespace msgpackpp
{
    packer& serialize(packer &p, const T &t)
    {
        return p.pack_?(t);
    }
}
```

# deserializer

use ``operator>>``

```cpp
```

## define deserializer

```cpp
namespace msgpackpp
{
    parser deserialize(const parser &u, T &t)
    {
        return u.get_?(t);
    }
}
```

# procedure call

