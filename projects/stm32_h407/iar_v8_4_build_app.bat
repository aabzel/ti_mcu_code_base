echo off
cls
set build_name=stm32_h407
echo build_name=%build_name%

set path_compiler="C:\IAR Systems\Embedded Workbench 8.1\common\bin\IarBuild.exe"
set workspace_name=C:\1_job\0_NPP_IT_ELMA\code_base_workspace\ti_mcu_code_base\projects\stm32_h407\stm32_h407.ewp


%path_compiler% %workspace_name% -build %build_name% -log all
Sleep 1



