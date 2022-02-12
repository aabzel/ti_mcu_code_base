#include "common_functions.h"

#include <inttypes.h>

#ifdef HAS_FREE_RTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#endif

#ifdef HAS_CLOCK
#include "clocks.h"
#endif
#ifdef HAS_LED
#include "led_drv.h"
#endif /*HAS_LED*/

#ifdef HAS_ADC
#include "adc_drv.h"
#endif /*HAS_ADC*/

#ifdef HAS_BOOT
#include "boot_driver.h"
#endif

#ifdef HAS_CLI
#include "cli_manager.h"
#endif /*HAS_CLI*/

#ifdef HAS_DAC
#include "dac_drv.h"
#endif /*HAS_DAC*/

#ifdef HAS_GNSS
#include "gnss_drv.h"
#endif /*HAS_GNSS*/

#ifdef HAS_GPIO
#include "gpio_drv.h"
#endif

#ifdef HAS_GPIO_PWM
#include "gpio_pwm_drv.h"
#endif /*HAS_GPIO_PWM*/

#ifdef HAS_HEALTH_MONITOR
#include "health_monitor.h"
#endif

#ifdef HAS_TBFP
#include "tbfp_re_tx_ack_fsm.h"
#endif

#ifdef HAS_CAN
#include "can_drv.h"
#endif /*HAS_CAN*/

#include "hw_init.h"
#ifdef HAS_LOG
#include "io_utils.h"
#include "log.h"
#endif
#include "sw_init.h"
#include "sys_config.h"
#ifdef HAS_TASKS
#include "task_info.h"
#endif
#ifdef HAS_UART
#include "uart_drv.h"
#endif

#ifdef HAS_FLASH_FS
#include "flash_fs.h"
#endif /*HAS_FLASH_FS*/

#ifdef HAS_LORA
#include "lora_drv.h"
#endif /*HAS_LORA*/

#ifdef HAS_PARAM
#include "param_ids.h"
#endif /*HAS_PARAM*/

#ifdef HAS_RF
#include "rf_drv.h"
#endif /*HAS_RF*/

#ifdef HAS_SX1262
#include "sx1262_drv.h"
#endif /*HAS_SX1262*/

#ifdef HAS_TCAN4550
#include "TCAN_hl.h"
#include "tcan4550_drv.h"
#endif /*HAS_TCAN4550*/

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

#ifdef HAS_RS232
#include "rs232_drv.h"
#endif /*HAS_RS232*/

bool sys_init(void) {
    bool res = true;
    res = try_init(hw_init(), "HW") && res;
    res = try_init(sw_init(), "SW") && res;
    return res;
}

#ifdef HAS_SUPER_LOOP
void super_loop(uint64_t loop_start_time_us) {
#ifdef HAS_DEBUG
    iteration_cnt++;
#endif
    if(0u == loop_start_time_us) {
        return;
    }
#ifdef HAS_WDT
    measure_task_interval(TASK_ID_WDT, 1000, proc_watchdog, loop_start_time_us);
#endif /*HAS_WDT*/

#if defined(HAS_LED) && !defined(HAS_FREE_RTOS)
    measure_task_interval(TASK_ID_LED, LED_POLL_PERIOD_US, proc_leds, loop_start_time_us);
#endif /*HAS_LED*/

#if defined(HAS_CLI) && !defined(HAS_FREE_RTOS)
    measure_task_interval(TASK_ID_CLI, 1000, cli_process, loop_start_time_us);
#endif /*HAS_CLI*/

#ifdef HAS_UART
    measure_task_interval(TASK_ID_UART, UART_PERIOD_US, proc_uarts, loop_start_time_us);
#endif /*HAS_UART1*/

#ifdef HAS_GENERIC
    measure_task_interval(TASK_ID_BOOT, BOOT_PERIOD_US, boot_proc, loop_start_time_us);
#endif

#ifdef HAS_DAC
    measure_task_interval(TASK_ID_DAC, 100000, dac_proc, loop_start_time_us);
#endif

#ifdef HAS_ADC
    measure_task_interval(TASK_ID_ADC, 300000, adc_proc, loop_start_time_us);
#endif /*HAS_ADC*/

#ifdef HAS_RS232
    measure_task_interval(TASK_ID_RS232, RS232_PERIOD_US, proc_rs232, loop_start_time_us);
#endif /*HAS_UART1*/

#ifdef HAS_TCAN4550
    // measure_task_interval(TASK_ID_TCAN4550, 500000, tcan_proc, loop_start_time_us);
    measure_task_interval(TASK_ID_TCAN4550, TCAN4550_PERIOD_US, tcan4550_proc, loop_start_time_us);
#endif /*HAS_TCAN4550*/

#ifdef HAS_CAN
    measure_task_interval(TASK_ID_CAN, CAN_PERIOD_US, can_proc, loop_start_time_us);
#endif /*HAS_TCAN4550*/

#ifdef HAS_UART0_FWD
    measure_task_interval(TASK_ID_UART0_FWD, 1, proc_uart0_fwd, loop_start_time_us);
#endif

#ifdef HAS_UART1_FWD
    measure_task_interval(TASK_ID_UART1_FWD, 1, proc_uart1_fwd, loop_start_time_us);
#endif


#ifdef HAS_RF
    measure_task_interval(TASK_ID_RF, 3000, rf_process, loop_start_time_us);
#endif /*HAS_RF*/

#if defined(HAS_SX1262) && !defined(HAS_FREE_RTOS)
    measure_task_interval(TASK_ID_SX1262, SX1262_PERIOD_US, sx1262_process, loop_start_time_us);
#endif /*HAS_SX1262*/

#ifdef HAS_LORA
    measure_task_interval(TASK_ID_LORA, LORA_PERIOD_US, lora_process, loop_start_time_us);
#endif /*HAS_SX1262*/

#ifdef HAS_TBFP
    measure_task_interval(TASK_ID_TBFP, TBFP_PERIOD_US, tbfp_retx_proc, loop_start_time_us);
#endif /*HAS_TBFP*/

#ifdef HAS_NMEA
    measure_task_interval(TASK_ID_NMEA, NMEA_PERIOD_US, nmea_proc, loop_start_time_us);
#endif /*HAS_NMEA*/
#ifdef HAS_UBLOX
    measure_task_interval(TASK_ID_UBX, UBX_PERIOD_US, ubx_proc, loop_start_time_us);
#endif /*HAS_UBLOX*/

#ifdef HAS_ZED_F9P
    measure_task_interval(TASK_ID_ZED_F9P, ZED_F9P_PERIOD_US, zed_f9p_proc, loop_start_time_us);
#endif /*HAS_SX1262*/

#ifdef HAS_GNSS
    measure_task_interval(TASK_ID_GNSS, GNSS_PERIOD_US, gnss_proc, loop_start_time_us);
#endif /*HAS_GNSS*/

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

#ifdef HAS_SUPER_LOOP
_Noreturn void super_main_loop(void) {
#ifdef HAS_CLOCK
#ifdef HAS_LOG
    io_printf("Main Task started, up time: %u ms" CRLF, get_time_ms32());
#endif
#else
#ifdef HAS_LOG
    io_printf("Main Task started" CRLF);
#endif
#endif
#ifdef HAS_CLOCK
    uint64_t loop_start_time_us = 0;
#endif
#ifdef HAS_DEBUG
    uint64_t prev_loop_start_time_us = 0;
#endif /*HAS_DEBUG*/
    for(;;) {
#ifdef HAS_CLOCK
        loop_start_time_us = 0;
        loop_start_time_us = get_time_us();
#endif /*HAS_CLOCK*/
#ifdef HAS_DEBUG
#ifdef LAUNCHXL_CC26X2R1
        gpio_toggle(COM_LOOP_SENSOR_DIO_NO);
#endif /*LAUNCHXL_CC26X2R1*/
#ifdef HAS_CLOCK
        loop_duration_us = loop_start_time_us - prev_loop_start_time_us;
        loop_duration_min_us = rx_min64u(loop_duration_min_us, loop_duration_us);
        loop_duration_max_us = rx_max64u(loop_duration_max_us, loop_duration_us);
#endif
        prev_loop_start_time_us = loop_start_time_us;
#endif /*HAS_DEBUG*/
#ifdef HAS_TASKS
        super_loop(loop_start_time_us);
#endif
#ifdef HAS_FREE_RTOS
        // taskYIELD();
        vTaskDelay(5 / portTICK_PERIOD_MS);
#endif
    }
}
#endif /*NORTOS*/

bool try_init(bool status, char* message) {
    if(false == status) {
#ifdef HAS_LOG
        LOG_ERROR(HMOM, "init %s error", message);
#endif
    } else {
#ifdef HAS_LOG
        LOG_INFO(HMOM, "init %s OK", message);
#endif
    }
    return status;
}
