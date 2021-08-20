add_executable(test msgpack.cpp serialize.cpp rpc.cpp stream.cpp request.cpp)
target_link_libraries(test PRIVATE catch msgpackpp)
