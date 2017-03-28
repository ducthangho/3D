tasklist /FI "IMAGENAME eq redis-server.exe" 2>NUL | find /I /N "redis-server.exe">NUL
if "%ERRORLEVEL%"=="0" (
	echo redis-server.exe is running	
)else (
	start C:\ProgramData\chocolatey\bin\redis-server.exe
)

tasklist /FI "IMAGENAME eq LogServer.exe" 2>NUL | find /I /N "LogServer.exe">NUL
if "%ERRORLEVEL%"=="0" (
	echo LogServer.exe is running	
)else (
	start F:\WorkSpace\3D\y3d\bin\LogServer.exe
)

tasklist /FI "IMAGENAME eq YMasterServer.exe" 2>NUL | find /I /N "YMasterServer.exe">NUL
if "%ERRORLEVEL%"=="0" (
	echo YMasterServer.exe is running	
)else (
	start F:\WorkSpace\3D\y3d\bin\mserver\YMasterServer.exe
)

tasklist /FI "IMAGENAME eq 3dsmax.exe" 2>NUL | find /I /N "3dsmax.exe">NUL
if "%ERRORLEVEL%"=="0" (
	echo 3dsmax.exe is running	
)else (
	start /D "C:\Program Files\Autodesk\3ds Max 2017\" 3dsmax.exe
)