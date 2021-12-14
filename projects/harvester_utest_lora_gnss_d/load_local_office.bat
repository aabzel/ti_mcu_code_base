echo off
echo %0
set build_name=harvester_utest_lora_gnss_d
set project_dir=%cd%
set workspace_dir=%project_dir%\..\..\

for %%f in (Debug\*.out) do (
    if "%%~xf"==".out" 	set find_artifact_out=%%f  
)

echo find_artifact_out=%find_artifact_out%

set artifact_out=%project_dir%\Debug\%build_name%_PATTERN.out

echo artifact_out=%artifact_out%
%workspace_dir%\tool\load_fw.bat C:\ti\uniflash_6.3.0 %project_dir% %artifact_out%