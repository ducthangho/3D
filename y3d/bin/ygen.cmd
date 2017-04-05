call %Y3D_ROOT%\bin\ygrpc ymax
call %Y3D_ROOT%\bin\ygrpc yservice
call %Y3D_ROOT%\bin\yproto y3d
call %Y3D_ROOT%\bin\yproto xnormal
call %Y3D_ROOT%\bin\yproto ymat
call %Y3D_ROOT%\bin\yproto ysetting
call %Y3D_ROOT%\bin\ygrpc yloader
call %Y3D_ROOT%\bin\yproto yloader

call %Y3D_ROOT%\bin\yproto ylogservice LOG_EXPORT_MACRO
call %Y3D_ROOT%\bin\ygrpc ylogservice LOG_EXPORT_MACRO
call %Y3D_ROOT%\bin\yproto ymaxcoreinterface

