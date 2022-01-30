@echo off
cls
set com_num=5
set work_space_dir=%cd%\..\..\
taskkill /IM ttermpro.exe
call %work_space_dir%\tool\launch_terminal.bat %com_num% 921600 MeshTastic







