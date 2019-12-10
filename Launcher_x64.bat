@echo off
cd .\bin\x64\Release\
start main.exe %*
if %ERRORLEVEL% NEQ 0 ( pause )