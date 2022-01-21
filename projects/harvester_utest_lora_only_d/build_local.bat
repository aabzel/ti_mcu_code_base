echo off
set project_dir=%cd%
set workspace_dir=%project_dir%\..\..\

%workspace_dir%tool\build.bat %workspace_dir% C:\ti\ccs1040\ccs\eclipse\eclipsec harvester_utest_lora_only_d

