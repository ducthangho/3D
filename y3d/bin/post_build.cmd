rem %1\..\x64\Release\LoaderCmd.exe -l 0
echo xcopy %1\..\lib\grpc\*.dll %2 /D /Y
xcopy %1\..\lib\grpc\*.dll %2 /D /Y
echo %1\..\bin\LoaderCmd.exe -l 1
%1\..\bin\LoaderCmd.exe -l 1