echo off
echo %0
echo Start ship build
set workspace_dir=%1
echo build_workspace_dir=%workspace_dir%

echo Copy project to workspace
xcopy /s /E /I /Y  %workspace_dir%  %workspace_dir%\..\code_base_ship 

