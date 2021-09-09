#include "common_functions.h"

#include <inttypes.h>

#include "clocks.h"

#ifdef HAS_LED
#include "led_drv.h"
#endif /*HAS_LED*/

#ifdef HAS_CLI
#include "cli_manager.h"
#endif /*HAS_CLI*/
#include "health_monitor.h"
#include "hw_init.h"
#include "io_utils.h"
#include "log.h"
#include "sw_init.h"
//#include "task_config.h"
#include "task_info.h"
#include "uart_drv.h"

#ifdef HAS_SX1262
#include "sx1262_drv.h"
#endif /*HAS_SX1262*/

#ifdef HAS_ADC
#include "adc_drv.h"
#endif /*HAS_ADC*/

#ifdef HAS_DAC
#include "dac_drv.h"
#endif /*HAS_DAC*/

#ifdef HAS_FLASH_FS
#include "flash_fs.h"
#endif /*HAS_FLASH_FS*/

#ifdef HAS_RF
#include "rf_drv.h"
#endif /*HAS_RF*/

#ifdef HAS_UBLOX
#include "ublox_driver.h"
#include "ubx_protocol.h"
#endif /*HAS_UBLOX*/

#ifdef HAS_NMEA
#include "nmea_protocol.h"
#endif /*HAS_NMEA*/

#ifdef HAS_WDT
#include "watchdog_drv.h"
#endif

bool sys_init(void) {
    bool res = true;
    res = try_init(hw_init(), "HW") && res;
    res = try_init(sw_init(), "SW") && res;
    return res;
}

void common_loop(uint64_t loop_start_time_us) {
    if(0u == loop_start_time_us) {
        return;
    }

#ifdef HAS_DAC
    measure_task_interval(TASK_ID_DAC, 100000, dac_proc, loop_start_time_us);
#endif

#ifdef HAS_ADC
    measure_task_interval(TASK_ID_ADC, 100000, adc_proc, loop_start_time_us);
#endif

#ifdef HAS_WDT
    measure_task_interval(TASK_ID_WDT, 1000, proc_watchdog, loop_start_time_us);
#endif

#ifdef HAS_LED
    measure_task_interval(TASK_ID_LED, 5000, proc_led, loop_start_time_us);
#endif /*HAS_LED*/

#ifdef HAS_UART1
    measure_task_interval(TASK_ID_UART1, 2000, proc_uart1, loop_start_time_us);
#endif /*HAS_UART1*/

#ifdef HAS_UBLOX
    measure_task_interval(TASK_ID_UBX, 10000, ubx_proc_frame, loop_start_time_us);
#endif /*HAS_UBLOX*/

#ifdef HAS_NMEA
    measure_task_interval(TASK_ID_NMEA, 500000, nmea_proc_message, loop_start_time_us);
#endif /*HAS_NMEA*/

#ifdef HAS_CLI
    measure_task_interval(TASK_ID_CLI, 3000, cli_process, loop_start_time_us);
#endif /*HAS_CLI*/

#ifdef HAS_RF
    measure_task_interval(TASK_ID_RF, 3000, rf_process, loop_start_time_us);
#endif /*HAS_RF*/

#ifdef HAS_SX1262
    measure_task_interval(TASK_ID_LORA, 100000, sx1262_process, loop_start_time_us);
#endif /*HAS_SX1262*/

#ifdef HAS_FLASH_FS
    measure_task_interval(TASK_ID_FLASH_FS, FLASH_FS_PERIOD_US, flash_fs_proc, loop_start_time_us);
#endif /*HAS_FLASH_FS*/
}

void common_main_loop(void) {
    io_printf("Main Task started, up time: %u ms" CRLF, get_time_ms32());
    uint64_t loop_start_time_us = 0;
    for(;;) {
        iteration_cnt++;
        loop_start_time_us = get_time_us();
        common_loop(loop_start_time_us);
    }
}
