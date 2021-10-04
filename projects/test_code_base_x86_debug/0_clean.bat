@echo off
set WORKSPACE_LOC = %cd%/../..
call %WORKSPACE_LOC%/clean_temp.bat

make clean

dir