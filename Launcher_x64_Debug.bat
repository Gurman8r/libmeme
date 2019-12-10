@echo off
cd .\bin\x64\Debug\
call main.exe %*
if %ERRORLEVEL% NEQ 0 ( pause )