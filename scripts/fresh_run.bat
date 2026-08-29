
:: Runs a clean build by following these steps:
:: 1. Compiles application
:: 2. Runs application

call "%~dp0compile.bat"

if %ERRORLEVEL% neq 0 (
    echo Failed to compile application with error code %ERRORLEVEL%
    exit /b %ERRORLEVEL%
)

call "%~dp0run.bat"
