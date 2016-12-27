%Y3D_ROOT%\bin\protoc -I=. --grpc_out=. --plugin=protoc-gen-grpc=%Y3D_ROOT%\bin\grpc_csharp_plugin.exe %1
%Y3D_ROOT%\bin\protoc -I=. --csharp_out=. %1