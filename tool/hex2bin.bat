echo off
cls
set hex3bin_tool=hex2bin.exe
set hex_file=%1
%hex3bin_tool% %hex_file%
rem C:/ti/simplelink_cc13x2_26x2_sdk_5_20_00_52/examples/rtos/CC26X2R1_LAUNCHXL/ble5stack/hexfiles/host_test_app.hex
rem %hex3bin_tool% -h