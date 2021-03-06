echo off
echo %0
echo Start build script....
echo .....
set workspace_dir=%1
echo build_workspace_dir=%workspace_dir%

set ide_tool_path=%2
echo ide_tool_path=%ide_tool_path%
set project_name=%3
echo project_name=%project_name%
set project_dir=%workspace_dir%projects\%project_name%

echo clean_artefacts
call %workspace_dir%\tool\clean_artefacts.bat %project_dir%
echo sign_code
call %workspace_dir%\tool\sign_code.bat %project_dir% 
set ide_tool=%ide_tool_path% -noSplash

echo Copy project to workspace
xcopy /s /E /I /Y  %workspace_dir%\projects\%project_name% %workspace_dir%\%project_name% 
echo check workspace content:
dir %workspace_dir%
echo Start IDE clean cmd
%ide_tool%  -data  %workspace_dir% -application com.ti.ccstudio.apps.projectBuild -ccs.projects %project_name% -ccs.clean  -ccs.autoImport -ccs.autoOpen
echo Start IDE build cmd
%ide_tool% -data %workspace_dir% -application com.ti.ccstudio.apps.projectBuild -ccs.projects %project_name% -ccs.buildType full -ccs.autoImport -ccs.autoOpen
sleep 15

echo Copy project from workspace to project dirr
xcopy /s /E /I /Y %workspace_dir%\%project_name% %workspace_dir%\projects\%project_name% 
echo Delete project from workspace
rmdir /s /q %workspace_dir%\%project_name%
COPY /Y  %workspace_dir%\auto_version_pattern.h %project_dir%\auto_version.h
call %workspace_dir%\tool\sign_artefacts.bat %workspace_dir% %project_name%

rem Links
rem http://software-dl.ti.com/ccs/esd/documents/ccs_projects-command-line.html