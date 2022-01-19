@echo off
cls
set com_num=%1
set work_space_dir=%cd%\..\..\
taskkill /IM ttermpro.exe
fw_configurator_d.exe COM%com_num% 460800 base.cfg
call %work_space_dir%\tool\launch_terminal.bat %com_num% base







