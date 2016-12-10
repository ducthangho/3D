protoc --grpc_out=. --plugin=protoc-gen-grpc=%Y3D_ROOT%\bin\grpc_csharp_plugin.exe %1
protoc --csharp_out=. %1