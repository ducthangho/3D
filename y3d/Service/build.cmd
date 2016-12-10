cd grpc\third_party\protobuf\cmake
mkdir build & cd build
cmake -G "Visual Studio 14 2015 Win64" -Dprotobuf_BUILD_SHARED_LIBS=ON -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_MSVC_STATIC_RUNTIME=OFF -Dprotobuf_WITH_ZLIB=OFF ..
msbuild /p:Configuration=Release protobuf.sln
xcopy Release\*.exe ..\..\..\..\..\..\bin\ /D /Y
xcopy Release\*.* ..\..\..\..\..\..\lib\protobuf\ /D /Y
cd ..
del /F /Q build

cd ..\
