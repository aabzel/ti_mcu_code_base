echo off
echo %0
set project_dir=%cd%
set workspace_dir=%project_dir%\..\
set uniflash_dir=C:\ti\uniflash_6.4.0

for %%f in (Debug\*.out) do (
    if "%%~xf"==".out" 	set artifact_out=%%f  
)

echo artifact_out=%artifact_out%
%workspace_dir%\tool\load_fw.bat %uniflash_dir% %project_dir% %artifact_out%