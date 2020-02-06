@echo off
cd .\bin\vs2019\x86\Debug\
call main.exe %*
if %ERRORLEVEL% NEQ 0 ( pause )