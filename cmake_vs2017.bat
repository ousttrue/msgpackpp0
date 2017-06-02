@echo off
FOR /F "TOKENS=1,2,*" %%A IN ('reg query "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\SxS\VS7" /v "15.0"') DO IF "%%A"=="15.0" SET VSPATH=%%C
@echo on

set CMAKE="%VSPATH%\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
mkdir build_vs2017
pushd build_vs2017
%CMAKE% .. -G "Visual Studio 15 2017"
popd
