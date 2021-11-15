#include "common_functions.h"

#include <inttypes.h>

#include "clocks.h"

#ifdef HAS_LED
#include "led_drv.h"
#endif /*HAS_LED*/

#ifdef HAS_CLI
#include "cli_manager.h"
#endif /*HAS_CLI*/

#include "boot_driver.h"

#ifdef HAS_GPIO
#include "gpio_drv.h"
#endif

#ifdef HAS_HEALTH_MONITOR
#include "health_monitor.h"
#endif

#include "hw_init.h"
#include "io_utils.h"
#include "log.h"
#include "sw_init.h"
#include "sys_config.h"
#ifdef NORTOS
#include "task_info.h"
#endif
#include "uart_drv.h"

#ifdef HAS_ADC
#include "adc_drv.h"
#endif /*HAS_ADC*/

#ifdef HAS_GPIO_PWM
#include "gpio_pwm_drv.h"
#endif /*HAS_GPIO_PWM*/

#ifdef HAS_DAC
#include "dac_drv.h"
#endif /*HAS_DAC*/

#ifdef HAS_FLASH_FS
#include "flash_fs.h"
#endif /*HAS_FLASH_FS*/

#ifdef HAS_PARAM
#include "param_ids.h"
#endif /*HAS_PARAM*/

#ifdef HAS_RF
#include "rf_drv.h"
#endif /*HAS_RF*/

#ifdef HAS_TCAN4550
#include "TCAN_hl.h"
#include "tcan4550_drv.h"
#endif /*HAS_TCAN4550*/

#ifdef HAS_SX1262
#include "sx1262_drv.h"
#endif /*HAS_SX1262*/

#ifdef HAS_LORA
#include "lora_drv.h"
#endif /*HAS_LORA*/

#ifdef HAS_UBLOX
#include "ublox_driver.h"
#include "ubx_protocol.h"
#endif /*HAS_UBLOX*/

#ifdef HAS_ZED_F9P
#include "zed_f9p_drv.h"
#endif

#ifdef HAS_NMEA
#include "nmea_protocol.h"
#endif /*HAS_NMEA*/

#ifdef HAS_WDT
#include "watchdog_drv.h"
#endif /*HAS_WDT*/

bool sys_init(void) {
    bool res = true;
    res = try_init(hw_init(), "HW") && res;
    res = try_init(sw_init(), "SW") && res;
    return res;
}

#ifdef NORTOS
void common_loop(uint64_t loop_start_time_us) {
    if(0u == loop_start_time_us) {
        return;
    }
#ifdef HAS_LED
    measure_task_interval(TASK_ID_LED, LED_POLL_PERIOD_US, proc_led, loop_start_time_us);
#endif /*HAS_LED*/

#ifdef HAS_CLI
    measure_task_interval(TASK_ID_CLI, 1000, cli_process, loop_start_time_us);
#endif /*HAS_CLI*/

#ifdef HAS_GENERIC
    measure_task_interval(TASK_ID_BOOT, 2000000, boot_proc, loop_start_time_us);
#endif

#ifdef HAS_DAC
    measure_task_interval(TASK_ID_DAC, 100000, dac_proc, loop_start_time_us);
#endif

#ifdef HAS_ADC
    measure_task_interval(TASK_ID_ADC, 100000, adc_proc, loop_start_time_us);
#endif

#ifdef HAS_WDT
    measure_task_interval(TASK_ID_WDT, 1000, proc_watchdog, loop_start_time_us);
#endif /*HAS_WDT*/

#ifdef HAS_UART
    measure_task_interval(TASK_ID_UART, 1, proc_uarts, loop_start_time_us);
#endif /*HAS_UART1*/

#ifdef HAS_TCAN4550
    // measure_task_interval(TASK_ID_TCAN4550, 500000, tcan_proc, loop_start_time_us);
    measure_task_interval(TASK_ID_TCAN4550, 1000, tcan4550_proc, loop_start_time_us);
#endif /*HAS_TCAN4550*/

#ifdef HAS_UART0_FWD
    measure_task_interval(TASK_ID_UART0_FWD, 1, proc_uart0_fwd, loop_start_time_us);
#endif

#ifdef HAS_UART1_FWD
    measure_task_interval(TASK_ID_UART1_FWD, 1, proc_uart1_fwd, loop_start_time_us);
#endif

#ifdef HAS_UBLOX
    measure_task_interval(TASK_ID_UBX, UBX_PERIOD_US, ubx_proc, loop_start_time_us);
#endif /*HAS_UBLOX*/

#ifdef HAS_RF
    measure_task_interval(TASK_ID_RF, 3000, rf_process, loop_start_time_us);
#endif /*HAS_RF*/

#ifdef HAS_SX1262
    measure_task_interval(TASK_ID_SX1262, SX1262_PERIOD_US, sx1262_process, loop_start_time_us);
#endif /*HAS_SX1262*/

#ifdef HAS_LORA
    measure_task_interval(TASK_ID_LORA, LORA_PERIOD_US, lora_process, loop_start_time_us);
#endif /*HAS_SX1262*/

#ifdef HAS_NMEA
    measure_task_interval(TASK_ID_NMEA, NMEA_PERIOD_US, nmea_proc, loop_start_time_us);
#endif /*HAS_SX1262*/

#ifdef HAS_ZED_F9P
    measure_task_interval(TASK_ID_ZED_F9P, ZED_F9P_PERIOD_US, zed_f9p_proc, loop_start_time_us);
#endif /*HAS_SX1262*/

#ifdef HAS_FLASH_FS
    measure_task_interval(TASK_ID_FLASH_FS, FLASH_FS_PERIOD_US, flash_fs_proc, loop_start_time_us);
#endif /*HAS_FLASH_FS*/

#ifdef HAS_GPIO_PWM
    measure_task_interval(TASK_ID_GPIO_PWM, GPIO_PWM_PERIOD_US, gpio_pwm_proc, loop_start_time_us);
#endif /*HAS_FLASH_FS*/

#ifdef HAS_HEALTH_MONITOR
    measure_task_interval(TASK_ID_HEAL_MON, HEAL_MON_PERIOD_US, health_monotor_proc, loop_start_time_us);
#endif /*HAS_HEALTH_MONITOR*/

#ifdef HAS_PARAM
    measure_task_interval(TASK_ID_PARAM, PARAM_PERIOD_US, param_proc, loop_start_time_us);
#endif /*HAS_PARAM*/

}
#endif /*NORTOS*/

#ifdef NORTOS
_Noreturn void common_main_loop(void) {
    io_printf("Main Task started, up time: %u ms" CRLF, get_time_ms32());
    uint64_t loop_start_time_us = 0;
#ifdef HAS_DEBUG
    uint64_t prev_loop_start_time_us = 0;
#endif /*HAS_DEBUG*/
    for(;;) {
        loop_start_time_us = get_time_us();
#ifdef HAS_DEBUG
        iteration_cnt++;
#ifdef LAUNCHXL_CC26X2R1
        gpio_toggle(COM_LOOP_SENSOR_DIO_NO);
#endif /*LAUNCHXL_CC26X2R1*/
        loop_duration_us = loop_start_time_us - prev_loop_start_time_us;
        loop_duration_min_us = rx_min64u(loop_duration_min_us, loop_duration_us);
        loop_duration_max_us = rx_max64u(loop_duration_max_us, loop_duration_us);
        prev_loop_start_time_us = loop_start_time_us;
#endif /*HAS_DEBUG*/
        common_loop(loop_start_time_us);
    }
}
#endif /*NORTOS*/

bool try_init(bool status, char* message) {
    if(false == status) {
        LOG_ERROR(HMOM, "init %s error", message);
    } else {
        LOG_INFO(HMOM, "init %s OK", message);
    }
    return status;
}
