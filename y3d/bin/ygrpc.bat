%~d0
cd %Y3D_ROOT%\bin\scheme
%Y3D_ROOT%\bin\protoc -I=. --proto_path=%Y3D_ROOT%\Service\grpc\third_party\protobuf\src --grpc_out=. --plugin=protoc-gen-grpc=%Y3D_ROOT%\bin\grpc_cpp_plugin.exe %1.proto
if [%2]==[] (
%Y3D_ROOT%\bin\protoc -I=. --proto_path=%Y3D_ROOT%\Service\grpc\third_party\protobuf\src --cpp_out=dllexport_decl=Y3D_EXPORT_MACRO:. %1.proto
) else (
%Y3D_ROOT%\bin\protoc -I=. --proto_path=%Y3D_ROOT%\Service\grpc\third_party\protobuf\src --cpp_out=dllexport_decl=%2:. %1.proto
)

%Y3D_ROOT%\bin\protoc -I=. --grpc_out=. --proto_path=%Y3D_ROOT%\Service\grpc\third_party\protobuf\src --plugin=protoc-gen-grpc=%Y3D_ROOT%\bin\grpc_csharp_plugin.exe %1.proto
%Y3D_ROOT%\bin\protoc -I=. --proto_path=%Y3D_ROOT%\Service\grpc\third_party\protobuf\src --csharp_out=. %1.proto

move /y *.cs %Y3D_ROOT%\Y3D.Entities\
move /y *.h %Y3D_ROOT%\include\
move /y *.cc %Y3D_ROOT%\ServiceImpl\

cd %Y3D_ROOT%\bin\