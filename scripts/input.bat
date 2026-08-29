
@echo off

set "ARG1=%~1"

if "%~2"=="" set "ARG2=2"
if not "%~2"=="" set "ARG2=%~2"

if "%ARG1%"=="" (
    echo. >CON
) else (
    echo %ARG1%  >CON
)
timeout /t %ARG2% /nobreak >nul

if "%ARG1%"=="" (
    echo.
) else (
    echo %ARG1%
)
timeout /t %ARG2% /nobreak >nul

exit /b
