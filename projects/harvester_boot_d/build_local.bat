echo off
set project_dir=%cd%
set workspace_dir=%project_dir%\..\..\
echo workspace_dir=%workspace_dir%
set project_name=harvester_boot_d
%workspace_dir%tool\build.bat %workspace_dir% C:\ti\ccs1040\ccs\eclipse\eclipsec %project_name%

