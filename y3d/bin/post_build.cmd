rem %1\..\x64\Release\LoaderCmd.exe -l 0
echo xcopy %1..\lib\grpc\*.dll %2 /D /Y
xcopy %1..\lib\grpc\*.dll %2 /D /Y
xcopy %1..\lib\grpc\*.pdb %2 /D /Y
echo xcopy %1..\lib\protobuf\*.dll %2 /D /Y
xcopy %1..\lib\protobuf\*.dll %2 /D /Y
xcopy %1..\lib\protobuf\*.pdb %2 /D /Y
echo xcopy %1..\lib\tbb\*.dll %2 /D /Y
xcopy %1..\lib\tbb\*.dll %2 /D /Y
xcopy %1..\lib\tbb\*.pdb %2 /D /Y
rem echo %ERRORLEVEL%
rem echo %1\..\bin\LoaderCmd.exe -l 1
%1\..\bin\LoaderCmd.exe -l 1