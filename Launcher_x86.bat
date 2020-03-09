@echo off
cd .\bin\vs2019\x86\Release\
start main.exe %*
if %ERRORLEVEL% NEQ 0 ( pause )