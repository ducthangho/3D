net stop "YMasterService"
sc delete  "YMasterService"
if  %ERRORLEVEL% NEQ 0 (
    echo "Error level is %errorlevel%"
    exit /b 0
)