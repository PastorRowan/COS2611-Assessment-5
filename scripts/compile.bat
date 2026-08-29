
:: Compiles the Municipal Incident Response System source code.
::
:: This script changes to the project root directory, compiles the C++ source
:: file using the GNU C++ compiler, produces 25939831_FPP.exe as the executable, and
:: then restores the previous working directory.

:: Change the current working directory to the parent directory of this batch file.
pushd "%~dp0.."

:: Compile the C++ source file.
::
:: g++:
:: GNU C++ compiler.
::
:: -std=c++20:
:: Compile the program using the C++20 language standard.
::
:: 25939831_FPP.cpp:
:: C++ source file containing the Municipal Incident Response System.
::
:: -o 25939831_FPP.exe:
:: Specifies the name and location of the output executable.
::
:: The resulting executable is 25939831_FPP.exe

g++ -std=c++20 25939831_FPP.cpp -o 25939831_FPP.exe


:: Restore the previous working directory.
popd
