cls
echo off
rem dslite flash --config=ccxml-file [options] [flash-file1 ...]
rem set flash_tool=C:\ti\uniflash_6.4.0\uniflash\public\file_package_creator\templates\win\dslite.bat
set config_file=C:\1_job\0_NPP_IT_ELMA\code_base_workspace2\ti_mcu_code_base\tool\CC2652R1F.ccxml
set hex_fw_file=C:\ti\simplelink_cc13x2_26x2_sdk_5_20_00_52\examples\rtos\CC26X2R1_LAUNCHXL\ble5stack\hexfiles\project_zero_unsecure_app.hex
set flash_tool=C:\ti\uniflash_6.4.0\dslite.bat
rem %flash_tool% -h
%flash_tool%  --config %config_file% -e -f %hex_fw_file%