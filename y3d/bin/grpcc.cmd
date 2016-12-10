protoc --grpc_out=. --plugin=protoc-gen-grpc=%Y3D_ROOT%\bin\grpc_cpp_plugin.exe %1
protoc --cpp_out=.  %1