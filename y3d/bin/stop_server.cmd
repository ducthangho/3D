echo taskkill /F /IM "%1"
taskkill /F /IM "%1" 2>NUL
if  %ERRORLEVEL% NEQ 0 (
    echo "Error level is %errorlevel%"
    exit /b 0
)