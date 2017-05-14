rem set redis-server-path=C:\ProgramData\chocolatey\bin\redis-server.exe
rem set log-server-path=F:\WorkSpace\3D\y3d\bin\LogServer.exe
rem set YMasterServer-path=F:\WorkSpace\3D\y3d\bin\mserver\YMasterServer.exe
set redis-server-path="C:\ProgramData\chocolatey\bin\"
set log-server-path="C:\Program Files\Autodesk\3ds Max 2017\"
set YMasterServer-path="%Y3D_ROOT%\bin\mserver\"
set threedsmax-path="C:\Program Files\Autodesk\3ds Max 2017\"
set LoaderCmd-path="%Y3D_ROOT%\bin"
set Y3D-path="%Y3D_ROOT%\bin\Release"
echo %threedsmax-path%

rem set a=
rem echo %a%


tasklist /FI "IMAGENAME eq redis-server.exe" 2>NUL | find /I /N "redis-server.exe">NUL
if "%ERRORLEVEL%"=="0" (	
	taskkill /f /im  redis-server.exe
)else (
	echo redis-server.exe was not running
)

rem tasklist /FI "IMAGENAME eq LogServer.exe" 2>NUL | find /I /N "LogServer.exe">NUL
rem if "%ERRORLEVEL%"=="0" (
rem 	taskkill /f /im  LogServer.exe
rem )else (
rem 	echo LogServer.exe was not running	
rem )

tasklist /FI "IMAGENAME eq YMasterServer.exe" 2>NUL | find /I /N "YMasterServer.exe">NUL
if "%ERRORLEVEL%"=="0" (
	taskkill /f /im  YMasterServer.exe
)else (
	echo YMasterServer.exe was not running	
)

tasklist /FI "IMAGENAME eq 3dsmax.exe" 2>NUL | find /I /N "3dsmax.exe">NUL
if "%ERRORLEVEL%"=="0" (
	taskkill /f /im  3dsmax.exe
)else (
	echo 3dsmax.exe was not running	
)

tasklist /FI "IMAGENAME eq Y3D.exe" 2>NUL | find /I /N "Y3D.exe">NUL
if "%ERRORLEVEL%"=="0" (
	taskkill /f /im  Y3D.exe
)else (
	echo Y3D-path was not running	
)
