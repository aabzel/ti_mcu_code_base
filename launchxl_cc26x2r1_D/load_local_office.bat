echo off
echo %0
set project_dir=%cd%
set workspace_dir=%project_dir%\..\

for %%f in (Debug\*.out) do (
    if "%%~xf"==".out" 	set artifact_out=%%f  
)

echo artifact_out=%artifact_out%
%workspace_dir%\tool\load_fw.bat C:\ti\uniflash_6.3.0 %project_dir% %artifact_out%