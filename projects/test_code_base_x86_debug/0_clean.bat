@echo off
set WORKSPACE_LOC=%cd%/../..
echo WORKSPACE_LOC=%WORKSPACE_LOC%
call %WORKSPACE_LOC%/clean_temp.bat

make clean

dir