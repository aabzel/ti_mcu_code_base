echo off
set project_dir=%cd%
set workspace_dir=%project_dir%\..\..\

%workspace_dir%tool\build.bat %workspace_dir% C:\ti\ccs1040\ccs\eclipse\eclipsec project_zero_CC26X2R1_LAUNCHXL_tirtos_ccs

