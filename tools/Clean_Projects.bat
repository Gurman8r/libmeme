@echo off

if exist ..\.vs rd /s /q ..\.vs
if exist ..\proj rd /s /q ..\proj

if exist ..\libmeme_vs2017.sln del /q ..\libmeme_vs2017.sln
if exist ..\libmeme_vs2019.sln del /q ..\libmeme_vs2019.sln

if %ERRORLEVEL% NEQ 0 ( pause ) else ( echo projects OK )