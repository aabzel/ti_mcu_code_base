#include "common_functions.h"

#include <inttypes.h>

#include "clocks.h"

#ifdef HAS_LED
#include "led_drv.h"
#endif /*HAS_LED*/

#ifdef HAS_CLI
#include "cli_manager.h"
#endif /*HAS_CLI*/

#include "io_utils.h"
#include "log.h"
#include "task_config.h"
#include "task_info.h"
#include "uart_drv.h"

#ifdef HAS_ADC
#include "adc_drv.h"
#endif

#ifdef HAS_DAC
#include "dac_drv.h"
#endif

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

void common_loop(uint64_t loop_start_time) {
    if(0u == loop_start_time) {
        return;
    }

#ifdef HAS_DAC
    MEASURE_TASK_INTERVAL(DAC, 1, dac_proc);
#endif

#ifdef HAS_ADC
    MEASURE_TASK_INTERVAL(ADC, 100, adc_proc);
#endif

#ifdef HAS_WDT
    MEASURE_TASK_INTERVAL(WDT, 1, proc_watchdog);
#endif

#ifdef HAS_LED
    MEASURE_TASK_INTERVAL(LED, 1, proc_led);
#endif /*HAS_LED*/

    MEASURE_TASK_INTERVAL(UART1, 1, proc_uart1);

#ifdef HAS_UBLOX
    MEASURE_TASK_INTERVAL(UBX, 1, ubx_proc_frame);
#endif /*HAS_UBLOX*/

#ifdef HAS_NMEA
    MEASURE_TASK_INTERVAL(NMEA, 1, nmea_proc_message);
#endif /*HAS_NMEA*/

#ifdef HAS_CLI
    MEASURE_TASK_INTERVAL(CLI, 30, cli_process);
#endif /*HAS_CLI*/

#ifdef HAS_RF
    MEASURE_TASK_INTERVAL(RF, 30, rf_process);
#endif /*HAS_RF*/
}

uint32_t g_iteration_cnt = 10;

void common_main_loop(void) {
    io_printf("Main Task started, up time: %u ms" CRLF, get_time_ms32());

    for(;;) {
        g_iteration_cnt++;
        MAIN_LOOP_START;
        common_loop(loop_start_time);
    }
}
