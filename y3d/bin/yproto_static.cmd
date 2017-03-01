%~d0
cd %Y3D_ROOT%\bin\scheme

%Y3D_ROOT%\bin\protoc -I=. --proto_path=%Y3D_ROOT%\Service\grpc\third_party\protobuf\src --cpp_out=. %1.proto
%Y3D_ROOT%\bin\protoc -I=. --proto_path=%Y3D_ROOT%\Service\grpc\third_party\protobuf\src --csharp_out=. %1.proto

xcopy /y *.cs %Y3D_ROOT%\..\MaxNet\Y3D\Y3D.Entities\
move /y *.cs %Y3D_ROOT%\y3d\Models\
move /y *.h %Y3D_ROOT%\include\
move /y *.cc %Y3D_ROOT%\ServiceImpl\

cd %Y3D_ROOT%\bin\