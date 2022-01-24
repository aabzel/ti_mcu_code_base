echo off
echo %0
cls
set workspace_dir=%cd%/../
echo build_workspace_dir=%workspace_dir%

set static_analysis_tool_path="C:\Program Files\Cppcheck\cppcheck.exe"
echo ide_tool_path=%static_analysis_tool_path%

%static_analysis_tool_path% %workspace_dir%