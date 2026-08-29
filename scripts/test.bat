
REM Municipal Incident Response System - Automated Test Script

REM PURPOSE:
REM Automatically tests the menu functionality of 25939831_FPP.exe by supplying
REM predefined keyboard input through standard input (stdin).
REM
REM The script dynamically generates an input.bat helper script. Each call
REM to input.bat simulates a user entering a menu option and optionally
REM waits for a specified amount of time before sending the input to
REM 25939831_FPP.exe.
REM
REM The generated input.bat is required because each test input needs to
REM behave similarly to keyboard input entered by a real user.
REM
REM EXECUTION FLOW:
REM
REM 1. Move to the directory containing this test script.
REM 2. Configure the delay between simulated inputs.
REM 3. Generate input.bat.
REM 4. Move to the parent directory containing 25939831_FPP.exe.
REM 5. Execute the predefined test sequence.
REM 6. Pipe the simulated input into 25939831_FPP.exe.
REM 7. Restore the original working directory.

@echo on

REM pushd is used instead of cd so that the original working directory can be restored later using popd.
pushd "%~dp0"

REM Number of seconds to wait between automatically supplied inputs.
set /A TIMEOUT=4

REM Generate empty input.bat file.
type nul > input.bat

REM The input.bat file is generated dynamically using echo commands.
REM
REM The doubled percent signs (%%) are intentional. Since this script is
REM generating another batch file, %% is required so that the generated file
REM receives a single % character.
REM
REM The ^ characters escape parentheses and redirection operators so that they
REM are written into input.bat rather than being interpreted by this script.

(
    echo @echo off
    echo.
    echo set "ARG1=%%~1"
    echo.
    echo if "%%~2"=="" set "ARG2=4"
    echo if not "%%~2"=="" set "ARG2=%%~2"
    echo.
    echo if "%%ARG1%%"=="" ^(
    echo     echo. ^>CON
    echo ^) else ^(
    echo     echo %%ARG1%% ^>CON
    echo ^)
    echo timeout /t %%ARG2%% /nobreak ^>nul
    echo.
    echo if "%%ARG1%%"=="" ^(
    echo     echo.
    echo ^) else ^(
    echo     echo %%ARG1%%
    echo ^)
    echo timeout /t %%ARG2%% /nobreak ^>nul
    echo.
    echo exit /b
) > input.bat

REM Move to the parent directory.
REM
REM 25939831_FPP.exe is expected to be located one directory above this test script.
pushd "%~dp0.."

REM AUTOMATED TEST SEQUENCE
REM
REM All commands inside this parenthesised block produce input for 25939831_FPP.exe.
REM
REM The pipe:
REM (
REM ...
REM ) | 25939831_FPP.exe ...
REM
REM redirects the standard output produced by the block into the standard input
REM of 25939831_FPP.exe.
REM
REM Each call to input.bat supplies one simulated user input.
REM
REM call "%~dp0input.bat" "7"
REM
REM simulates entering 7
REM
REM
REM call "%~dp0input.bat" "" simulates pressing ENTER without entering a value.
REM
REM IMPORTANT:
REM REM. is used instead of :: or REM for comments inside this piped command block.
REM :: is technically a label rather than a real comment and can cause
REM parsing problems inside parenthesised/piped batch blocks and REM also does not work.
(

    timeout /t %TIMEOUT% /nobreak >nul

    REM. Display Locations and Roads menu test
    call "%~dp0input.bat" "1"
    call "%~dp0input.bat" ""

    REM. Display Incidents menu test
    call "%~dp0input.bat" "2"
    call "%~dp0input.bat" ""

    REM. Prioritise Open Incidents menu test
    call "%~dp0input.bat" "3"
    call "%~dp0input.bat" ""

    REM. Display Response Teams menu test
    call "%~dp0input.bat" "4"
    call "%~dp0input.bat" ""

    REM. Investigate Incident menu test
    call "%~dp0input.bat" "5"
    call "%~dp0input.bat" "1"
    call "%~dp0input.bat" ""

    REM. Investigate Incident menu test
    call "%~dp0input.bat" "6"
    call "%~dp0input.bat" "0"
    call "%~dp0input.bat" "1"
    call "%~dp0input.bat" ""

    REM. Find Route menu test
    call "%~dp0input.bat" "7"
    call "%~dp0input.bat" "1"
    call "%~dp0input.bat" "1"
    call "%~dp0input.bat" ""

    REM. Update Incident menu test

    REM. Assign incident response team test
    call "%~dp0input.bat" "7"
    call "%~dp0input.bat" "1"
    call "%~dp0input.bat" "1"
    call "%~dp0input.bat" "3"
    call "%~dp0input.bat" ""

    REM. Unassign incident response team test
    call "%~dp0input.bat" "7"
    call "%~dp0input.bat" "1"
    call "%~dp0input.bat" "1"
    call "%~dp0input.bat" ""

    REM. Reassign incident response team test
    call "%~dp0input.bat" "7"
    call "%~dp0input.bat" "1"
    call "%~dp0input.bat" "1"
    call "%~dp0input.bat" "3"
    call "%~dp0input.bat" ""

    REM. Resolve incident test
    call "%~dp0input.bat" "7"
    call "%~dp0input.bat" "1"
    call "%~dp0input.bat" "2"
    call "%~dp0input.bat" ""

    REM. Reopen incident test
    call "%~dp0input.bat" "7"
    call "%~dp0input.bat" "1"
    call "%~dp0input.bat" "1"
    call "%~dp0input.bat" ""

    REM. Cancel incident update test
    call "%~dp0input.bat" "7"
    call "%~dp0input.bat" "1"
    call "%~dp0input.bat" "0"
    call "%~dp0input.bat" ""

    REM. Display Summary menu test
    call "%~dp0input.bat" "8"
    call "%~dp0input.bat" ""

    REM. Exit application
    call "%~dp0input.bat" "0"

) | 25939831_FPP.exe 1 6 10

REM Restore the previous working directory.
REM
REM Two pushd commands were used above, so two corresponding popd commands
REM are required to restore the original directory.

popd
popd
