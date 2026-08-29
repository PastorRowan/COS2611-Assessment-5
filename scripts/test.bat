
@echo off

pushd "%~dp0.."

set /A TIMEOUT=4

(

    timeout /t %TIMEOUT% /nobreak >nul

    call "%~dp0input.bat" "7"
    call "%~dp0input.bat" "1"
    call "%~dp0input.bat" "1"
    call "%~dp0input.bat" "3"
    call "%~dp0input.bat" ""

    call "%~dp0input.bat" "7"
    call "%~dp0input.bat" "1"
    call "%~dp0input.bat" "1"
    call "%~dp0input.bat" ""

    call "%~dp0input.bat" "7"
    call "%~dp0input.bat" "1"
    call "%~dp0input.bat" "1"
    call "%~dp0input.bat" "3"
    call "%~dp0input.bat" ""

    call "%~dp0input.bat" "7"
    call "%~dp0input.bat" "1"
    call "%~dp0input.bat" "2"
    call "%~dp0input.bat" ""

    call "%~dp0input.bat" "7"
    call "%~dp0input.bat" "1"
    call "%~dp0input.bat" "1"
    call "%~dp0input.bat" ""

    call "%~dp0input.bat" "7"
    call "%~dp0input.bat" "1"
    call "%~dp0input.bat" "0"
    call "%~dp0input.bat" ""

    call "%~dp0input.bat" "0"

) | main.exe 1 6 10

    :: Display Locations and Roads menu test
    :: call "%~dp0input.bat" "1"
    :: call "%~dp0input.bat" ""

    :: Display Incidents menu test
    :: call "%~dp0input.bat" "2"
    :: call "%~dp0input.bat" ""

    :: Prioritise Open Incidents menu test
    :: call "%~dp0input.bat" "3"
    :: call "%~dp0input.bat" ""

    :: Display Response Teams menu test
    :: call "%~dp0input.bat" "4"
    :: call "%~dp0input.bat" ""

    :: Investigate Incident menu test
    :: call "%~dp0input.bat" "5"
    :: call "%~dp0input.bat" "1"
    :: call "%~dp0input.bat" ""

    :: Investigate Incident menu test
    :: call "%~dp0input.bat" "6"
    :: call "%~dp0input.bat" "0"
    :: call "%~dp0input.bat" "1"
    :: call "%~dp0input.bat" ""

    :: Find Route menu test
    :: call "%~dp0input.bat" "7"
    :: call "%~dp0input.bat" "1"
    :: call "%~dp0input.bat" "1"
    :: call "%~dp0input.bat" ""

    :: Update Incident menu test

    :: Assign incident response team
    :: call "%~dp0input.bat" "7"
    :: call "%~dp0input.bat" "1"
    :: call "%~dp0input.bat" "1"
    :: call "%~dp0input.bat" "3"
    :: call "%~dp0input.bat" ""

    :: call "%~dp0input.bat" "7"
    :: call "%~dp0input.bat" "1"
    :: call "%~dp0input.bat" "1"
    :: call "%~dp0input.bat" ""

    :: call "%~dp0input.bat" "7"
    :: call "%~dp0input.bat" "1"
    :: call "%~dp0input.bat" "1"
    :: call "%~dp0input.bat" "3"
    :: call "%~dp0input.bat" ""

    :: call "%~dp0input.bat" "7"
    :: call "%~dp0input.bat" "1"
    :: call "%~dp0input.bat" "2"
    :: call "%~dp0input.bat" ""

    :: call "%~dp0input.bat" "7"
    :: call "%~dp0input.bat" "1"
    :: call "%~dp0input.bat" "1"
    :: call "%~dp0input.bat" ""

    :: call "%~dp0input.bat" "7"
    :: call "%~dp0input.bat" "1"
    :: call "%~dp0input.bat" "0"
    :: call "%~dp0input.bat" ""

    :: call "%~dp0input.bat" "8"
    :: call "%~dp0input.bat" ""

    :: call "%~dp0input.bat" "0"

popd
