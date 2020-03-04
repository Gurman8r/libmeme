@echo off
cd .\bin\vs2019\x64\Debug\
call main.exe %*
if %ERRORLEVEL% NEQ 0 ( pause ) else ( echo "failed starting program" )