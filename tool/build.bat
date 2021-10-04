echo off
echo %0
echo Start build script
echo .....
set workspace_dir=%1
echo build_workspace_dir=%workspace_dir%

set ide_tool_path=%2
set project_name=%3
set project_dir=%workspace_dir%projects\%project_name%
rem set  ide_tool=C:\ti\ccs1040\ccs\eclipse\jre\bin\java.exe startup.jar
call %workspace_dir%\tool\clean_artefacts.bat %project_dir%
call %workspace_dir%\tool\sign_code.bat %project_dir% 
rem set  ide_tool=C:\ti\ccs1040\ccs\eclipse\eclipsec -noSplash
set  ide_tool=%ide_tool_path% -noSplash
%ide_tool% -data %workspace_dir% -application com.ti.ccstudio.apps.projectBuild -ccs.projects %project_name%  -ccs.buildType full -ccs.autoImport
COPY /Y  %workspace_dir%\auto_version_pattern.h %project_dir%\auto_version.h
call %workspace_dir%\tool\sign_artefacts.bat %workspace_dir% %project_name%
rem .\jre\bin\java -jar startup.jar -data <workspace_path> -application com.ti.ccstudio.apps.projectCreate -ccs.name -ccs.device [-options]
rem eclipsec -noSplash -data "<workspace_dir>" -application com.ti.ccstudio.apps.projectBuild (-ccs.projects <name1>[ <name2>]* | -ccs.workspace) [<options>]
rem eclipsec   -noSplash -data "<workspace_dir>" -application com.ti.ccstudio.apps.projectBuild (-ccs.projects <name1>[ <name2>]* | -ccs.workspace) [<options>]