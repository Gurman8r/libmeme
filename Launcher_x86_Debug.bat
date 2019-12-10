@echo off
cd .\bin\x86\Debug\
call main.exe %*
if %ERRORLEVEL% NEQ 0 ( pause )