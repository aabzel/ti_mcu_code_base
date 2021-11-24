echo off
cls
set project_dir=%cd%
set workspace_dir=%project_dir%\..\..\

%workspace_dir%tool\build.bat %workspace_dir% C:\ti\ccs1030\ccs\eclipse\eclipsec harvester_utest_hw_gnss_d