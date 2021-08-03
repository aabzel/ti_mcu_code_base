echo off

rem C:\ti\uniflash_6.3.0
rem C:\ti\uniflash_6.4.0
set uniflash_dir=%1
%uniflash_dir%\dslite.bat --mode load --config=%cd%\targetConfigs\CC2652R1F.ccxml   %cd%\Debug\launchxl_cc26x2r1_D.out

