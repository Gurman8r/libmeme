@echo off
cd .\bin\x86\Release\
start main.exe %*
if %ERRORLEVEL% NEQ 0 ( pause )