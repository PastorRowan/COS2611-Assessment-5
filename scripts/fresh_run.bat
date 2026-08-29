
@echo off


:: Runs a clean build by following these steps:
:: 1. Compiles application
:: 2. Runs application

call "%~compile.bat"
call "%~dp0run.bat"
