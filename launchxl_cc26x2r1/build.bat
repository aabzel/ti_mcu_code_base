echo off

rem set  ide_tool=C:\ti\ccs1040\ccs\eclipse\jre\bin\java.exe startup.jar
set  ide_tool=C:\ti\ccs1040\ccs\eclipse\eclipsec -noSplash
set  workspace_dir=%cd%\..\
echo workspace_dir=%workspace_dir%
set  project_name=launchxl_cc26x2r1
%ide_tool% -data %workspace_dir% -application com.ti.ccstudio.apps.projectBuild -ccs.projects %project_name%  -ccs.buildType full 

rem .\jre\bin\java -jar startup.jar -data <workspace_path> -application com.ti.ccstudio.apps.projectCreate -ccs.name -ccs.device [-options]
rem eclipsec -noSplash -data "<workspace_dir>" -application com.ti.ccstudio.apps.projectBuild (-ccs.projects <name1>[ <name2>]* | -ccs.workspace) [<options>]
rem eclipsec   -noSplash -data "<workspace_dir>" -application com.ti.ccstudio.apps.projectBuild (-ccs.projects <name1>[ <name2>]* | -ccs.workspace) [<options>]