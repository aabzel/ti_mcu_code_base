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
    MEASURE_TASK_INTERVAL(DAC, 100000, dac_proc);
#endif

#ifdef HAS_ADC
    MEASURE_TASK_INTERVAL(ADC, 100000, adc_proc);
#endif

#ifdef HAS_WDT
    MEASURE_TASK_INTERVAL(WDT, 1000, proc_watchdog);
#endif

#ifdef HAS_LED
    MEASURE_TASK_INTERVAL(LED, 5000, proc_led);
#endif /*HAS_LED*/

#ifdef HAS_UART1
    MEASURE_TASK_INTERVAL(UART1, 2000, proc_uart1);
#endif /*HAS_UART1*/

#ifdef HAS_UBLOX
    MEASURE_TASK_INTERVAL(UBX, 10000, ubx_proc_frame);
#endif /*HAS_UBLOX*/

#ifdef HAS_NMEA
    MEASURE_TASK_INTERVAL(NMEA, 500000, nmea_proc_message);
#endif /*HAS_NMEA*/

#ifdef HAS_CLI
    MEASURE_TASK_INTERVAL(CLI, 3000, cli_process);
#endif /*HAS_CLI*/

#ifdef HAS_RF
    MEASURE_TASK_INTERVAL(RF, 3000, rf_process);
#endif /*HAS_RF*/

#ifdef HAS_SX1262
    MEASURE_TASK_INTERVAL(LORA, 100000, sx1262_process);
#endif /*HAS_SX1262*/

#ifdef HAS_FLASH_FS
    MEASURE_TASK_INTERVAL(FLASH_FS, FLASH_FS_PERIOD_US, flash_fs_proc);
#endif /*HAS_FLASH_FS*/
}



void common_main_loop(void) {
    io_printf("Main Task started, up time: %u ms" CRLF, get_time_ms32());

    for(;;) {

        MAIN_LOOP_START;
        common_loop(loop_start_time_us);
    }
}
