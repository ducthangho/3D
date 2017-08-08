cd grpc\
md .build
cd .build
cmake .. -G "Visual Studio 14 2015" -DCMAKE_BUILD_TYPE=Release "-DCMAKE_TOOLCHAIN_FILE=F:\git\vcpkg\scripts\buildsystems\vcpkg.cmake"
cmake --build .
