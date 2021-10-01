@echo off

set work_space_dir=%cd%
set style1="{SpaceBeforeParens: Never, IndentWidth: 4, SortIncludes: true, SortUsingDeclarations: true , UseTab: Never,PointerAlignment: Left, BreakBeforeBraces: Attach, TabWidth: 4,ColumnLimit: 120 }"
set format_tool="C:\Program Files\LLVM\bin\clang-format.exe"
rem set format_tool="C:\cygwin64\bin\clang-format.exe"
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\fw_loader_debug\main.c

%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_sw\test_fifo\test_fifo_char.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_sw\test_fifo\test_fifo_char.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\fifo_char_buffer.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\fifo_char_buffer.h

%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_sw\test_fifo\test_fifo_indexer.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_sw\test_fifo\test_fifo_indexer.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_hw\test_uart\test_uart.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_hw\test_uart\test_uart.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\stm32f413zgj6_d\Core\Src\freertos_init.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\stm32f413zgj6_d\Core\Src\freertos_init.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\bq25171_q1_drv\bq25171_q1_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\bq25171_q1_drv\bq25171_q1_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\pwr_mux\pwr_mux_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\pwr_mux\pwr_mux_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\ext_ram_simulator\ext_ram_sim_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\ext_ram_simulator\ext_ram_sim_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\ext_ram_simulator\ext_ram_sim_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\ext_ram_simulator\ext_ram_sim_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\win_utils\scan_serial_port.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\win_utils\scan_serial_port.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\params\param_types.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\params\param_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\params\param_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\params\param_ids.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\params\param_ids.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\cli_drv\task_info.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\cli_drv\task_info.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\flash_fs\flash_fs.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\flash_fs\flash_fs.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\flash_fs\flash_fs_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\flash_fs\flash_fs_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\sx1262\gfsk_defs.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\sx1262\lora_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\sx1262\lora_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\sx1262\lora_defs.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\sx1262\lora_handler.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\sx1262\lora_handler.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\sx1262\sx1262_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\sx1262\sx1262_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\sx1262\sx1262_diag.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\sx1262\sx1262_diag.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\sx1262\sx1262_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\sx1262\sx1262_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\adc\adc_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\adc\adc_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\adc\adc_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\adc\adc_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\boot_drv\boot_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\boot_drv\boot_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\boot_drv\boot_driver.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\boot_drv\boot_driver.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\cli_drv\base_cmd.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\cli_drv\base_cmd.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\cli_drv\cli_manager.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\cli_drv\cli_manager.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\cli_drv\common_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\cli_drv\shell_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\clocks\clock_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\clocks\clock_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\clocks\clocks.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\clocks\clocks.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\core_drv\core_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\core_drv\core_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\dac\dac_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\dac\dac_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\dac\dac_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\dac\dac_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\flash\flash_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\flash\flash_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\flash\flash_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\flash\flash_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\gpio\gpio_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\gpio\gpio_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\gpio\gpio_diag.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\gpio\gpio_diag.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\gpio\gpio_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\gpio\gpio_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\gpio\led_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\gpio\led_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\gpio\led_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\gpio\led_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\i2c\i2c_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\i2c\i2c_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\i2c\i2c_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\i2c\i2c_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\rf\rf_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\rf\rf_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\rf\rf_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\rf\rf_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\rng\rng_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\rng\rng_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\rng\rng_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\rng\rng_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\spi\spi_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\spi\spi_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\spi\spi_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\spi\spi_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\tim\tim_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\tim\tim_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\tim\tim_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\tim\tim_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\uart\uart_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\uart\uart_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\uart\uart_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\uart\uart_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\watchdog\watchdog_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\watchdog\watchdog_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\watchdog\watchdog_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\watchdog\watchdog_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\common\common_functions.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\common\common_functions.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\main.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\nmea_protocol\nmea_protocol.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\nmea_protocol\nmea_protocol.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\ubx_protocol\ublox_command.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\ubx_protocol\ublox_command.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\ubx_protocol\ublox_driver.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\ubx_protocol\ublox_driver.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\ubx_protocol\ubx_protocol.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\ubx_protocol\ubx_protocol.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set\test_crc\test_crc.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set\test_crc\test_crc.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set\test_string\test_convert.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set\test_string\test_convert.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set\test_string\text_convert.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set\test_string\text_convert.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set\test_sx1262\test_sx1262.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set\test_sx1262\test_sx1262.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\unit_test_check.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set\test_flash_fs\test_flash_fs.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set\test_flash_fs\test_flash_fs.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set\test_list.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\bit_utils.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\bit_utils.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\crc\crc16_ccitt.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\crc\crc16_ccitt.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\crc\crc32.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\crc\crc32.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\crc\crc8_autosar.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\crc\crc8_autosar.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\crc\crc8_sae_j1850.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\crc\crc8_sae_j1850.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\debug_info.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\debug_info.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\health_monitor.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\health_monitor.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\read_mem.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\read_mem.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\time_utils.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\time_utils.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\utils_string\convert.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\utils_string\convert.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\utils_string\str_utils.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\utils_string\str_utils.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\utils_sys\diag_sys.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\utils_sys\diag_sys.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\utils_file\utils_file.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\utils_file\utils_file.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\data_utils.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\data_utils.h
