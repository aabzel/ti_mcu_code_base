echo off

set BUILD_NAME=stm32_h407
echo BUILD_NAME=%BUILD_NAME%

set path_compiler="C:\Program Files (x86)\IAR Systems\Embedded Workbench 8.4\common\bin\IarBuild.exe"

%path_compiler% stm32_h407.ewp -build %BUILD_NAME% -log all
Sleep 1



