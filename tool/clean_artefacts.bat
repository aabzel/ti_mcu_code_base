echo off
echo %0
set project_dir=%1
echo project_dir=%project_dir%
del %project_dir%\Debug\*.bin
del %project_dir%\Debug\*.hex
del %project_dir%\Debug\*.map
del %project_dir%\Debug\*.out

