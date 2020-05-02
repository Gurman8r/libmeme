@echo off
cd ..\
call .\premake5\bin\Windows\premake5.exe vs2019
if %ERRORLEVEL% NEQ 0 ( pause )