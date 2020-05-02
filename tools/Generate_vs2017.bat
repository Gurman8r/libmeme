@echo off
cd ..\
call .\premake5\bin\Windows\premake5.exe vs2017
if %ERRORLEVEL% NEQ 0 ( pause )