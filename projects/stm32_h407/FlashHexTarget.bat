set StLinkCliTool="C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe"
set FirmwareBinPath=C:\1_job\0_NPP_IT_ELMA\code_base_workspace\ti_mcu_code_base\projects\stm32_h407\stm32_h407\Exe\stm32_h407.bin
set FirmwareHexPath=C:\1_job\0_NPP_IT_ELMA\code_base_workspace\ti_mcu_code_base\projects\stm32_h407\stm32_h407\Exe\stm32_h407.hex
%StLinkCliTool% -c SN=37FF6F064D56343143480943 SWD UR FREQ=400 -P %FirmwareHexPath% -V -HardRst

