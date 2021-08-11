echo off
echo %0
rem C:\ti\uniflash_6.3.0
rem C:\ti\uniflash_6.4.0
set uniflash_dir=%1
set project_dir=%2
set out_file_path=%3
rem %uniflash_dir%\dslite.bat --mode load --config=%cd%\targetConfigs\CC2652R1F.ccxml   %cd%\Debug\launchxl_cc26x2r1_D.out
%uniflash_dir%\dslite.bat --mode load --config=%project_dir%\targetConfigs\CC2652R1F.ccxml %out_file_path%


