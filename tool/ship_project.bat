echo off
echo %0
echo Start ship build
set workspace_dir=%1
echo build_workspace_dir=%workspace_dir%
set project_name=%2
echo project_name=%project_name%
echo Copy project to workspace
xcopy /s /E /I /Y  %workspace_dir%\projects\%project_name% %workspace_dir%\..\code_base_ship\projects\%project_name% 
echo check ship workspace content:
dir %workspace_dir%\..\code_base_ship
