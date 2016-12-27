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

rem cd ../../../../bin/zlib/release
rem set PATH=%PATH%;%cd%\bin

popd
pushd %~dp0

copy tracing.c grpc\src\core\ext\census
cd grpc\third_party\protobuf\cmake

del /Q CMakeCache.txt
cmake -G "Visual Studio 14 2015 Win64" -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_WITH_ZLIB=ON
rem devenv.com protobuf.sln /build "Debug|x64" /project ALL_BUILD
rem if not %ERRORLEVEL% == 0 goto Finish
rem robocopy /mir .\Debug ..\..\..\bin\protobuf\debug
rem robocopy /mir .\Release build\solution\Debug

devenv.com protobuf.sln /build "Release|x64" /project ALL_BUILD
rem msbuild /m /p:Configuration=Release protobuf.sln
if not %ERRORLEVEL% == 0 goto Finish
robocopy /mir .\Release ..\..\..\bin\protobuf\release
robocopy /mir .\Release ..\..\..\..\..\lib\protobuf
robocopy /mir .\Release build\solution\Release
rem devenv.com protobuf.sln /clean "Debug|x64"
rem msbuild protobuf.sln /t:Clean
devenv.com protobuf.sln /clean "Release|x64"

cd ..\..\..\vsprojects
rem devenv.com grpc_protoc_plugins.sln /build "Release|x64"
msbuild /m /p:Configuration=Release /p:Platform=x64 /p:WarningLevel=0 /p:TreatWarningsAsErrors=false grpc_protoc_plugins.sln
if not %ERRORLEVEL% == 0 goto Finish
robocopy .\x64\Release\ ..\bin\grpc_protoc_plugins\ /XF *.lib *.iobj *.ipdb
robocopy .\x64\Release\ ..\..\..\bin\ /XF *.lib *.iobj *.ipdb
rem devenv.com grpc_protoc_plugins.sln /clean "Release|x64"
msbuild grpc_protoc_plugins.sln /t:Clean

rem devenv.com grpc.sln /clean "Debug"
msbuild grpc.sln /t:Clean /p:Configuration=Debug
rem devenv.com grpc.sln /clean "Release"
rem devenv.com grpc.sln /build "Debug|x64" /project grpc_dll
rem devenv.com grpc.sln /build "Debug|x64" /project grpc++
rem devenv.com grpc.sln /build "Debug|x64" /project grpc++_unsecure
rem if not %ERRORLEVEL% == 0 goto Finish
rem robocopy /mir .\x64\Debug ..\bin\grpc\debug

copy /Y "..\..\grpc++_unsecure.vcxproj" "vcxproj\grpc++_unsecure\"
copy /Y "..\..\grpc++.vcxproj" "vcxproj\grpc++\"
rem devenv.com grpc.sln /build "Release|x64" /project grpc_dll
msbuild /m /p:Configuration=Release /p:Platform=x64 /p:WarningLevel=0 /p:TreatWarningAsErrors=false grpc.sln /t:grpc_dll
rem devenv.com grpc.sln /build "Release|x64" /project grpc++
msbuild /m /p:Configuration=Release /p:Platform=x64 /p:WarningLevel=0 /p:TreatWarningAsErrors=false grpc.sln /t:grpc++
rem devenv.com grpc.sln /build "Release|x64" /project grpc++_unsecure
msbuild /m /p:Configuration=Release /p:Platform=x64 /p:WarningLevel=0 /p:TreatWarningAsError=false grpc.sln /t:grpc++_unsecure
rem devenv.com grpc_csharp_ext.sln /build "Release|x64" /project grpc_csharp_ext
msbuild /m /p:Configuration=Release /p:Platform=x64 /p:WarningLevel=0 /p:TreatWarningsAsErrors=false grpc_csharp_ext.sln /t:grpc_csharp_ext
if not %ERRORLEVEL% == 0 goto Finish
robocopy /mir .\x64\Release ..\bin\grpc\release /XF *grpc_cpp_plugin*
robocopy /mir .\x64\Release ..\..\..\lib\grpc /XF *grpc_cpp_plugin*

rem devenv.com grpc.sln /clean "Debug"
msbuild grpc.sln /t:Clean /p:Configuration=Debug
rem devenv.com grpc.sln /clean "Release"
msbuild grpc.sln /t:Clean /p:Configuration=Release
rem devenv.com grpc.sln /build "Debug-DLL|x64" /project grpc++
rem devenv.com grpc.sln /build "Debug-DLL|x64" /project grpc++_unsecure
rem if not %ERRORLEVEL% == 0 goto Finish
rem robocopy /mir .\x64\Debug-DLL ..\bin\grpc\debug_dll

rem devenv.com grpc.sln /build "Release-DLL|x64" /project grpc++
msbuild grpc.sln /p:Configuration=Release-DLL /p:Platform=x64 /t:grpc++:clean
rem devenv.com grpc.sln /build "Release-DLL|x64" /project grpc++_unsecure
msbuild grpc.sln /p:Configuration=Release-DLL /p:Platform=x64 /t:grpc++_unsecure:clean
if not %ERRORLEVEL% == 0 goto Finish
robocopy /mir .\x64\Release-DLL ..\bin\grpc\release_dll /XF *grpc_cpp_plugin*
robocopy /mir .\x64\Release-DLL ..\..\..\lib\grpc\release_dll /XF *grpc_cpp_plugin*

cd ..\third_party\protobuf\cmake
del /Q CMakeCache.txt
mkdir build
cd build
del /Q CMakeCache.txt
cmake -G "Visual Studio 14 2015 Win64" -Dprotobuf_BUILD_SHARED_LIBS=ON -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_MSVC_STATIC_RUNTIME=OFF -Dprotobuf_WITH_ZLIB=OFF ..
msbuild /m /p:Configuration=Release protobuf.sln
xcopy "Release\*.*" "..\..\..\..\..\..\bin\" /Y
xcopy "Release\*.*" "..\..\..\..\..\..\lib\protobuf\" /Y
msbuild protobuf.sln /t:Clean
cd ..\..\..\..\..
..\bin\grpcc.cmd ymax.proto
..\bin\grpcc.cmd yservice.proto
..\bin\grpcc.cmd y3d.proto

echo #### grpc build done!

:Finish
rem devenv.com protobuf.sln /clean "Debug|x64"
rem devenv.com protobuf.sln /clean "Release|x64"
rem devenv.com grpc_protoc_plugins.sln /clean "Release|x64" /project grpc_cpp_plugin
rem devenv.com grpc.sln /clean "Debug|x64" /project grpc++
rem devenv.com grpc.sln /clean "Release|x64" /project grpc++
popd
pause