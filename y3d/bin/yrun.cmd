set redis-server-path="C:\ProgramData\chocolatey\lib\redis-64"
set log-server-path="%Y3D_ROOT%\bin"
set YMasterServer-path="%Y3D_ROOT%\bin\mserver\"
set threedsmax-path="C:\Program Files\Autodesk\3ds Max 2017\"
set LoaderCmd-path="%Y3D_ROOT%\bin"
set Y3D-path="%Y3D_ROOT%\y3d\bin\Release"
SET PWD=%cd%

REM tasklist /FI "IMAGENAME eq redis-server.exe" 2>NUL | find /I /N "redis-server.exe">NUL
REM if "%ERRORLEVEL%"=="0" (	
REM 	echo redis-server.exe is running	
REM )else (
REM 	start /D %redis-server-path% redis-server.exe
REM )

if not exist %redis-server-path% (
    choco install redis-64
)

sc query "redis" | find "RUNNING"
if %ERRORLEVEL% == 1 goto redis_stop
if %ERRORLEVEL% == 0 goto redis_running
echo unknown status
goto end

:redis_stop
net start "redis"
echo  %ERRORLEVEL%
if "%ERRORLEVEL%"=="2" (
    echo No service
    c: 
    cd %redis-server-path%
    start redis-server --service-install redis.windows.conf --loglevel verbose
    ping 127.0.0.1 -n 4 > nul
    net start "redis"
)
goto end
:redis_running
echo Redis is running
goto end

:end

cd "%PWD%"
tasklist /FI "IMAGENAME eq LogServer.exe" 2>NUL | find /I /N "LogServer.exe">NUL
if "%ERRORLEVEL%"=="0" (
	echo LogServer.exe is running	
)else (
	start /D %log-server-path% LogServer.exe
)

tasklist /FI "IMAGENAME eq YMasterServer.exe" 2>NUL | find /I /N "YMasterServer.exe">NUL
if "%ERRORLEVEL%"=="0" (
	echo YMasterServer.exe is running	
)else (
	start /D %YMasterServer-path% YMasterServer.exe
)

tasklist /FI "IMAGENAME eq Y3D.exe" 2>NUL | find /I /N "Y3D.exe">NUL
if "%ERRORLEVEL%"=="0" (	
	echo Y3D.exe is running	
)else (
	start /D %Y3D-path% Y3D.exe
)