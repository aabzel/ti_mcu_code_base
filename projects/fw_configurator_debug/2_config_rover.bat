@echo off
cls
set com_num=5
set work_space_dir=%cd%\..\..\
taskkill /IM ttermpro.exe
fw_configurator_d.exe COM%com_num% 460800 rover.cfg
call %work_space_dir%\tool\launch_terminal.bat %com_num% rover







