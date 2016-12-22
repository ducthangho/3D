@echo off
pushd %~dp0

call "%VS140COMNTOOLS%..\..\VC\vcvarsall.bat" amd64

echo #### grpc build start!

mkdir grpc\bin\zlib
rem mkdir grpc\bin\zlib\debug
mkdir grpc\bin\zlib\release

cd grpc\third_party\zlib
mkdir build & cd build
rem mkdir debug & cd debug
rem cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=../../../../bin/zlib/debug ../..
rem nmake & nmake install

rem cd ..
mkdir release & cd release
cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../../../bin/zlib/release ../..
nmake & nmake install

cd ../../../../bin/zlib/release
set PATH=%PATH%;%cd%\bin

popd
pushd %~dp0

cd grpc\third_party\protobuf\cmake
cmake -G "Visual Studio 14 2015 Win64" -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_WITH_ZLIB=ON
rem devenv.com protobuf.sln /build "Debug|x64" /project ALL_BUILD
rem if not %ERRORLEVEL% == 0 goto Finish
rem robocopy /mir .\Debug ..\..\..\bin\protobuf\debug
rem robocopy /mir .\Release build\solution\Debug

devenv.com protobuf.sln /build "Release|x64" /project ALL_BUILD
if not %ERRORLEVEL% == 0 goto Finish
robocopy /mir .\Release ..\..\..\bin\protobuf\release
robocopy /mir .\Release ..\..\..\..\..\lib\protobuf
robocopy /mir .\Release build\solution\Release
devenv.com protobuf.sln /clean "Debug|x64"
devenv.com protobuf.sln /clean "Release|x64"

cd ..\..\..\vsprojects
devenv.com grpc_protoc_plugins.sln /build "Release|x64"
if not %ERRORLEVEL% == 0 goto Finish
robocopy .\x64\Release\ ..\bin\grpc_protoc_plugins\ /XF *.lib *.iobj *.ipdb
robocopy .\x64\Release\ ..\..\..\bin\ /XF *.lib *.iobj *.ipdb
devenv.com grpc_protoc_plugins.sln /clean "Release|x64"

devenv.com grpc.sln /clean "Debug"
devenv.com grpc.sln /clean "Release"
rem devenv.com grpc.sln /build "Debug|x64" /project grpc_dll
rem devenv.com grpc.sln /build "Debug|x64" /project grpc++
rem devenv.com grpc.sln /build "Debug|x64" /project grpc++_unsecure
rem if not %ERRORLEVEL% == 0 goto Finish
rem robocopy /mir .\x64\Debug ..\bin\grpc\debug

devenv.com grpc.sln /build "Release|x64" /project grpc_dll
devenv.com grpc.sln /build "Release|x64" /project grpc++
devenv.com grpc.sln /build "Release|x64" /project grpc++_unsecure
if not %ERRORLEVEL% == 0 goto Finish
robocopy /mir .\x64\Release ..\bin\grpc\release /XF *grpc_cpp_plugin*
robocopy /mir .\x64\Release ..\..\..\lib\grpc /XF *grpc_cpp_plugin*

devenv.com grpc.sln /clean "Debug"
devenv.com grpc.sln /clean "Release"
rem devenv.com grpc.sln /build "Debug-DLL|x64" /project grpc++
rem devenv.com grpc.sln /build "Debug-DLL|x64" /project grpc++_unsecure
rem if not %ERRORLEVEL% == 0 goto Finish
rem robocopy /mir .\x64\Debug-DLL ..\bin\grpc\debug_dll

devenv.com grpc.sln /build "Release-DLL|x64" /project grpc++
devenv.com grpc.sln /build "Release-DLL|x64" /project grpc++_unsecure
if not %ERRORLEVEL% == 0 goto Finish
robocopy /mir .\x64\Release-DLL ..\bin\grpc\release_dll /XF *grpc_cpp_plugin*
robocopy /mir .\x64\Release-DLL ..\..\..\lib\grpc\release_dll /XF *grpc_cpp_plugin*

echo #### grpc build done!

:Finish
rem devenv.com protobuf.sln /clean "Debug|x64"
rem devenv.com protobuf.sln /clean "Release|x64"
devenv.com grpc_protoc_plugins.sln /clean "Release|x64" /project grpc_cpp_plugin
devenv.com grpc.sln /clean "Debug|x64" /project grpc++
devenv.com grpc.sln /clean "Release|x64" /project grpc++
popd
pause