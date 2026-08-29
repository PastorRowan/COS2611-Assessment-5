
@echo off

(
    echo 1
    timeout /t 2 /nobreak >nul

    echo 5
    timeout /t 2 /nobreak >nul

    echo 3
    timeout /t 2 /nobreak >nul

    echo 0
) | "%~dp0run.bat"

pause
