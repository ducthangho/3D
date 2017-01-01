call %Y3D_ROOT%\bin\ygrpc ymax
call %Y3D_ROOT%\bin\ygrpc yservice
call %Y3D_ROOT%\bin\yproto y3d
%Y3D_ROOT%\bin\protoc -I=%Y3D_ROOT%\bin\scheme --proto_path=%Y3D_ROOT%\Service\grpc\third_party\protobuf\src --cpp_out=%Y3D_ROOT%\bin\gen %Y3D_ROOT%\bin\scheme\xnormal.proto