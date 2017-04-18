rem set redis-server-path=C:\ProgramData\chocolatey\bin\redis-server.exe
rem set log-server-path=F:\WorkSpace\3D\y3d\bin\LogServer.exe
rem set YMasterServer-path=F:\WorkSpace\3D\y3d\bin\mserver\YMasterServer.exe
set redis-server-path="C:\ProgramData\chocolatey\bin\"
set log-server-path="C:\Program Files\Autodesk\3ds Max 2017\"
set YMasterServer-path="%Y3D_ROOT%\bin\mserver\"
set threedsmax-path="C:\Program Files\Autodesk\3ds Max 2017\"
set LoaderCmd-path="%Y3D_ROOT%\bin"
set Y3D-path="%Y3D_ROOT%\y3d\bin\Release"
echo %threedsmax-path%

rem set a=
rem echo %a%


rem tasklist /FI "IMAGENAME eq redis-server.exe" 2>NUL | find /I /N "redis-server.exe">NUL
rem if "%ERRORLEVEL%"=="0" (	
rem 	echo redis-server.exe is running	
rem )else (
rem 	start /D %redis-server-path% redis-server.exe
rem )

rem tasklist /FI "IMAGENAME eq LogServer.exe" 2>NUL | find /I /N "LogServer.exe">NUL
rem if "%ERRORLEVEL%"=="0" (
rem 	echo LogServer.exe is running	
rem )else (
rem 	start /D %log-server-path% LogServer.exe
rem )

rem tasklist /FI "IMAGENAME eq YMasterServer.exe" 2>NUL | find /I /N "YMasterServer.exe">NUL
rem if "%ERRORLEVEL%"=="0" (
rem 	echo YMasterServer.exe is running	
rem )else (
rem 	start /D %YMasterServer-path% YMasterServer.exe
rem )
TASKLIST /FI "IMAGENAME EQ 3DSMAX.EXE" 2>NUL | FIND /I /N "3DSMAX.EXE">NUL
IF "%ERRORLEVEL%"=="0" (
  	ECHO 3DSMAX.EXE IS RUNNING	
)ELSE (
  	START /D %THREEDSMAX-PATH% 3DSMAX.EXE
 )

rem tasklist /FI "IMAGENAME eq Y3D.exe" 2>NUL | find /I /N "Y3D.exe">NUL
rem if "%ERRORLEVEL%"=="0" (	
rem 	echo Y3D.exe is running	
rem )else (
rem 	start /D %Y3D-path% Y3D.exe
rem )


rem tasklist /FI "IMAGENAME eq LoaderCmd.exe" 2>NUL | find /I /N "LoaderCmd.exe">NUL
rem if "%ERRORLEVEL%"=="0" (
rem 	echo LoaderCmd.exe is running
rem )else (
rem 	start "??" /D %LoaderCmd-path% LoaderCmd.exe
rem )