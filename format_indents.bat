@echo off

set work_space_dir=%cd%
set style1="{SpaceBeforeParens: Never, IndentWidth: 4, SortIncludes: true, SortUsingDeclarations: true , UseTab: Never,PointerAlignment: Left, BreakBeforeBraces: Attach, TabWidth: 4,ColumnLimit: 120 }"
set format_tool="C:\Program Files\LLVM\bin\clang-format.exe"

%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\cli_drv\common_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\watchdog\watchdog_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\watchdog\watchdog_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\watchdog\watchdog_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\watchdog\watchdog_drv.c

%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\time_utils.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\time_utils.h

%format_tool%  -verbose -i -style=%style1% %work_space_dir%\common\common_functions.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\common\common_functions.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\clocks\clocks.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\clocks\clocks.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\cli_drv\shell_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\cli_drv\cli_manager.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\cli_drv\cli_manager.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\gpio\led_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\gpio\led_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\gpio\led_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\gpio\led_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\gpio\gpio_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\gpio\gpio_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\uart\uart_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\uart\uart_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\utils_string\convert.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\utils_string\convert.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set\test_string\test_convert.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set\test_string\test_convert.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\unit_test_check.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set\test_string\text_convert.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set\test_string\text_convert.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\nmea_protocol\nmea_protocol.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\nmea_protocol\nmea_protocol.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\ubx_protocol\ublox_command.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\ubx_protocol\ublox_command.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\ubx_protocol\ublox_driver.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\ubx_protocol\ublox_driver.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\ubx_protocol\ubx_protocol.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\ubx_protocol\ubx_protocol.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\launchxl_cc26x2r1\main.c

%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\utils_sys\diag_sys.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\utils_sys\diag_sys.c

%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\debug_info.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\debug_info.c

%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\core_drv\core_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\core_drv\core_commands.c

%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\cli_drv\shell.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\cli_drv\shell.c

%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\uart\uart_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\uart\uart_commands.c
