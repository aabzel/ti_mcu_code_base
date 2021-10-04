echo off
echo %0
echo Start build script
echo .....
set workspace_dir=%1
echo build_workspace_dir=%workspace_dir%

set ide_tool_path=%2
echo ide_tool_path=%ide_tool_path%
set project_name=%3
set project_dir=%workspace_dir%projects\%project_name%
call %workspace_dir%\tool\clean_artefacts.bat %project_dir%
call %workspace_dir%\tool\sign_code.bat %project_dir% 
set  ide_tool=%ide_tool_path% -noSplash
echo Start IDE build cmd
echo .....

%ide_tool% -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data %workspace_dir% -import %project_dir% -build all
rem %ide_tool% -data %workspace_dir% -application com.ti.ccstudio.apps.projectBuild -ccs.location %project_dir%  -ccs.projects projects//%project_name% -ccs.autoImport -ccs.buildType full

rem %ide_tool% -data %workspace_dir%   -application com.ti.ccstudio.apps.projectImport -ccs.location %project_dir% -ccs.buildType full
rem %ide_tool% -data %workspace_dir% -application com.ti.ccstudio.apps.projectBuild -ccs.workspace 
rem -ccs.location %project_dir%  -ccs.projects %project_name% -ccs.buildType full -ccs.listProblems
COPY /Y  %workspace_dir%\auto_version_pattern.h %project_dir%\auto_version.h
call %workspace_dir%\tool\sign_artefacts.bat %workspace_dir% %project_name%
rem .\jre\bin\java -jar startup.jar -data <workspace_path> -application com.ti.ccstudio.apps.projectCreate -ccs.name -ccs.device [-options]
rem eclipsec -noSplash -data "<workspace_dir>" -application com.ti.ccstudio.apps.projectBuild (-ccs.projects <name1>[ <name2>]* | -ccs.workspace) [<options>]
rem eclipsec   -noSplash -data "<workspace_dir>" -application com.ti.ccstudio.apps.projectBuild (-ccs.projects <name1>[ <name2>]* | -ccs.workspace) [<options>]