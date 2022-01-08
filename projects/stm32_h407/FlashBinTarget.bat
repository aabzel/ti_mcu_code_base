cls

set StLinkCliTool="C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe"
set FirmwareBinPath="%cd%\stm32_h407\Exe\stm32_h407.bin"
echo FirmwareBinPath=%FirmwareBinPath%
%StLinkCliTool% -c SN=37FF6F064D56343143480943 SWD UR FREQ=400 -P %FirmwareBinPath% 0x08000000
%StLinkCliTool% -Rst

rem see https://alex-exe.ru/radio/stm32/console-stm32-st-link-utility/

