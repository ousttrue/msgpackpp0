set CMAKE="D:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
mkdir build_vs2017
pushd build_vs2017
%CMAKE% .. -G "Visual Studio 15 2017"
popd
