@echo off
cd .\bin\vs2019\x64\Release\
start main.exe %*
if %ERRORLEVEL% NEQ 0 ( pause ) else ( echo "failed starting program" )