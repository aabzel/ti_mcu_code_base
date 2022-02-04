@echo off

set work_space_dir=%cd%
echo work_space_dir=%work_space_dir%
set style1="{SpaceBeforeParens: Never, IndentWidth: 4, SortIncludes: true, SortUsingDeclarations: true , UseTab: Never,PointerAlignment: Left, BreakBeforeBraces: Attach, TabWidth: 4,ColumnLimit: 120 }"
rem set format_tool="C:\Program Files\LLVM\bin\clang-format.exe"
set format_tool="C:\cygwin64\bin\clang-format.exe"

%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_esp32\flash_fs\flash_fs.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_esp32\flash_fs\flash_fs.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\protocol_diag.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\protocol_diag.h

%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\boot_drv\boot_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\bq25171_q1_drv\bq25171_q1_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\cli_drv\cli_manager.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\cli_drv\log_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\cli_drv\table_utils.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\cli_drv\table_utils.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\cli_drv\task_info.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\ext_ram_simulator\ext_ram_sim_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\ext_ram_simulator\ext_ram_sim_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\flash_fs\flash_fs.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\flash_fs\flash_fs.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\flash_fs\flash_fs_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\flash_fs\flash_fs_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\gpio_pwm\gpio_pwm_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\gpio_pwm\gpio_pwm_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\gpio_pwm\gpio_pwm_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\gpio_pwm\gpio_pwm_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\led_drv\led_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\led_drv\led_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\lora\lora_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\lora\lora_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\params\param_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\params\param_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\params\param_ids.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\params\param_ids.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\params\param_types.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\pwr_mux\pwr_mux_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\pwr_mux\pwr_mux_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\sx1262\gfsk_defs.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\sx1262\legal_band.c
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
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\tcan4550_q1\tcan4550_diag.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\tcan4550_q1\tcan4550_diag.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\tcan4550_q1\tcan4550_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\tcan4550_q1\tcan4550_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\tcan4550_q1\tcan4550_reg.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\tcan4550_q1\tcan4550_types.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\tcan4550_q1_ti\TCAN_hl.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\tcan4550_q1_ti\tcan4550_ti_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\tcan4550_q1_ti\tcan4550_ti_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\telematic\telematic_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\telematic\telematic_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\zed_f9p\zed_f9p_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\Drivers\zed_f9p\zed_f9p_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\adc\adc_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\adc\adc_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\boot_drv\boot_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\boot_drv\boot_driver.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\cli_drv\base_cmd.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\cli_drv\base_cmd.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\cli_drv\common_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\cli_drv\shell_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\clocks\clock_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\clocks\clocks.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\core_drv\core_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\core_drv\core_driver.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\core_drv\core_driver.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\dac\dac_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\dac\dac_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\dac\dac_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\flash\flash_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\flash\flash_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\gpio\gpio_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\gpio\gpio_diag.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\gpio\gpio_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\gpio\gpio_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\i2c\i2c_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\i2c\i2c_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\i2c\i2c_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\i2c\i2c_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\interrupt\interrupt_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\interrupt\interrupt_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\rf\rf_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\rf\rf_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\rng\rng_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\rng\rng_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\rtc\rtc_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\rtc\rtc_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\rtc\rtc_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\rtc\rtc_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\spi\spi_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\spi\spi_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\tim\tim_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\tim\tim_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\uart\uart_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\uart\uart_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\watchdog\watchdog_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_cc26x2\watchdog\watchdog_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_esp32\gpio\gpio_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_esp32\gpio\gpio_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_esp32\spi\spi_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_esp32\spi\spi_drv.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_stm32f4\clocks\clocks.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_stm32f4\clocks\sys_tick.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_stm32f4\spi\spi_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_stm32f4\spi\spi_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\bsp_stm32f4\uart\uart_drv.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\common\common_functions.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\common\sw_init.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\common\uart_common.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\common\uart_common.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\calendar\calendar.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\calendar\calendar.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\components\system\system.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\fw_loader_debug\main.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\main.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\projects\fw_loader_debug\cli_wrappers.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\projects\fw_loader_debug\fw_update.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\projects\fw_loader_debug\main.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\projects\fw_loader_debug\main.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\projects\harvester_generic_r\sys_config.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\projects\nmea_ploter_debug\main.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\projects\nmea_ploter_debug\nmea_plot.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\projects\nmea_ploter_debug\nmea_plot.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\projects\test_code_base_x86_debug\main.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\nmea_protocol\gnss_diag.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\nmea_protocol\nmea_diag.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\nmea_protocol\nmea_diag.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\nmea_protocol\nmea_protocol.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\nmea_protocol\nmea_protocol.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\protocol_common.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\rtcm3_protocol\rtcm3_protocol.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\rtcm3_protocol\rtcm3_protocol.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\tbfp_protocol\tbfp_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\tbfp_protocol\tbfp_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\tbfp_protocol\tbfp_protocol.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\tbfp_protocol\tbfp_protocol.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\tbfp_protocol\tbfp_protocol_parser.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\tbfp_protocol\tbfp_protocol_parser.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\ubx_protocol\ublox_command.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\ubx_protocol\ublox_command.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\ubx_protocol\ublox_driver.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\ubx_protocol\ublox_driver.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\ubx_protocol\ubx_diag.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\ubx_protocol\ubx_key_ids.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\ubx_protocol\ubx_protocol.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\protocols\ubx_protocol\ubx_protocol.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\stm32f413zgj6_d\Core\Src\freertos_init.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\stm32f413zgj6_d\Core\Src\freertos_init.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set\test_crc\test_crc.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set\test_crc\test_crc.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set\test_flash_fs\test_flash_fs.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set\test_flash_fs\test_flash_fs.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set\test_list.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set\test_string\test_convert.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set\test_string\test_convert.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set\test_string\text_convert.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set\test_string\text_convert.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set\test_sx1262\test_sx1262.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set\test_sx1262\test_sx1262.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_hw\test_gpio\test_gpio.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_hw\test_gpio\test_gpio.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_hw\test_uart\test_uart.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_hw\test_uart\test_uart.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_sw\test_calendar\test_calendar.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_sw\test_calendar\test_calendar.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_sw\test_fifo\test_fifo_array.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_sw\test_fifo\test_fifo_char.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_sw\test_fifo\test_fifo_char.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_sw\test_fifo\test_fifo_char.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_sw\test_fifo\test_fifo_char.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_sw\test_fifo\test_fifo_indexer.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_sw\test_fifo\test_fifo_indexer.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_sw\test_rtcm3_proto\test_rtcm3_proto.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_sw\test_rtcm3_proto\test_rtcm3_proto.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_sw\test_sw_list.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_sw\test_sw_list.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_sw\test_tbfp_proto\test_tbfp_proto.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_sw\test_tbfp_proto\test_tbfp_proto.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_sw\test_utils\test_array.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\test_set_sw\test_utils\test_array.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\unit_test.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\unit_test.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\unit_test_check.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\unit_test_commands.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\unit_test_commands.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\unit_test_run.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\unit_tests\unit_test_run.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\bit_utils.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\bit_utils.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\crc\crc16_ccitt.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\crc\crc16_ccitt.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\crc\crc24_q.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\crc\crc24_q.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\crc\crc32.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\crc\crc32.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\crc\crc8_autosar.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\crc\crc8_autosar.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\crc\crc8_sae_j1850.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\crc\crc8_sae_j1850.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\data_utils.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\data_utils.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\debug_info.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\debug_info.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\fifo\fifo_index.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\fifo_array.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\fifo_array.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\fifo_char.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\fifo_char.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\fifo_char_buffer.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\fifo_char_buffer.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\float_utils.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\float_utils.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\health_monitor.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\health_monitor.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\health_monitor_command.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\health_monitor_command.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\io_utils\io_utils.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\io_utils\io_utils.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\io_utils\log.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\io_utils\log.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\io_utils\oprintf.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\io_utils\ostream.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\io_utils\ostream.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\io_utils\print_format.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\io_utils\print_format.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\io_utils\string_writer.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\io_utils\uart_string_reader.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\io_utils\uart_string_reader.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\io_utils\writer_generic.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\io_utils\writer_lora.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\io_utils\writer_uart.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\io_utils\writer_uart.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\io_utils\writer_uart.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\none_blocking_pause.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\none_blocking_pause.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\read_mem.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\read_mem.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\time_utils.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\time_utils.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\utils_file\utils_file.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\utils_file\utils_file.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\utils_string\convert.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\utils_string\convert.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\utils_string\str_utils.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\utils_string\str_utils.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\utils_string\str_utils_ex.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\utils_sys\device_id.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\utils_sys\device_id.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\utils_sys\diag_sys.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\utils_sys\diag_sys.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\win_utils\cli_wrappers.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\utils\win_utils\cli_wrappers.h
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\win_utils\scan_serial_port.c
%format_tool%  -verbose -i -style=%style1% %work_space_dir%\win_utils\scan_serial_port.h
//%format_tool%  -verbose -i -style=%style1% %work_space_dir%\boards\harvester_v1\board_layout.c
//%format_tool%  -verbose -i -style=%style1% %work_space_dir%\boards\harvester_v1\board_layout.c
