set  ide_tool=C:\ti\ccs1040\ccs\eclipse\eclipsec.exe
set  workspace_dir="C:\projects\code_base"
set  project_name=launchxl_cc26x2r1_v2
%ide_tool% -noSplash -data . -application com.ti.ccstudio.apps.projectBuild -ccs.projects %project_name%  -ccs.buildType full -ccs.autoImport
rem eclipsec   -noSplash -data "<workspace_dir>" -application com.ti.ccstudio.apps.projectBuild (-ccs.projects <name1>[ <name2>]* | -ccs.workspace) [<options>]