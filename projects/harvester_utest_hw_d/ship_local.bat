echo off
set project_dir=%cd%
set workspace_dir=%project_dir%\..\..\

%workspace_dir%tool\ship_project.bat %workspace_dir% harvester_utest_hw_d
