
:: Runs a clean application build by following these steps:
:: 1. Compiles the application by executing compile.bat.
:: 2. If compilation fails, displays the error code and terminates.
:: 3. If compilation succeeds, executes run.bat to start the application.

:: Compile the application.
call "%~dp0compile.bat"

:: Terminate early if compilation failed.
if %ERRORLEVEL% neq 0 (
    echo Failed to compile application with error code %ERRORLEVEL%
    exit /b %ERRORLEVEL%
)

:: If compilation was successful, execute run.bat with all passed arguments supplied to this batch file
call "%~dp0run.bat" %*
