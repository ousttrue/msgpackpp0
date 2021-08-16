add_executable(sample sample.cpp) # .vcxproj
target_link_libraries(sample PRIVATE msgpackpp)
