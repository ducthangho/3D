LoaderCmd.exe -w startall 2>NUL
if  %ERRORLEVEL% NEQ 0 (
    echo "Error level is %errorlevel%"
    exit /b 0
)