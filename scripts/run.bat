
:: Runs the Municipal Incident Response System application executable.
:: The script temporarily changes the current working directory to the parent directory of this batch file,
:: runs 25939831_FPP.exe, and then restores the previous working directory.

:: Change the current working directory to the parent directory of this batch file.
pushd "%~dp0.."

:: Pass all arguments supplied to this batch file to the executable.
25939831_FPP.exe %*

:: Restore the previous working directory.
popd
