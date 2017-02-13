rem %1\..\x64\Release\LoaderCmd.exe -l 0
echo xcopy %1..\lib\grpc\grpc_dll.dll %2 /D /Y
xcopy %1..\lib\grpc\grpc_dll.dll %2 /D /Y
xcopy %1..\lib\grpc\grpc_dll.pdb %2 /D /Y
echo xcopy %1..\lib\protobuf\*.dll %2 /D /Y
xcopy %1..\lib\protobuf\*.dll %2 /D /Y
rem xcopy %1..\lib\protobuf\*.pdb %2 /D /Y
rem echo xcopy %1..\lib\tbb\*.dll %2 /D /Y
rem xcopy %1..\lib\tbb\tbb.dll %2 /D /Y
rem xcopy %1..\lib\tbb\tbbmalloc.dll %2 /D /Y
rem xcopy %1..\lib\tbb\tbb.pdb %2 /D /Y
rem xcopy %1..\lib\tbb\tbbmalloc.pdb %2 /D /Y
rem xcopy %1..\lib\tbb\*.pdb %2 /D /Y
rem echo %ERRORLEVEL%
rem echo %1\..\bin\LoaderCmd.exe -l 1

rem %1\..\bin\LoaderCmd.exe -l 1