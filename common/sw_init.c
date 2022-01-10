#include "sw_init.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef HAS_SUPER_LOOP
#include "common_functions.h"
#include "task_info.h"
#endif /*HAS_SUPER_LOOP*/

#ifdef HAS_EXT_RAM_EMUL
#include "ext_ram_sim_drv.h"
#endif /*HAS_EXT_RAM_EMUL*/

#ifdef HAS_BOOTLOADER
#include "boot_driver.h"
#endif /*HAS_BOOTLOADER*/

#ifdef HAS_LED
#include "led_drv.h"
#endif /*HAS_LED*/

#ifdef HAS_CALENDAR
#include "calendar.h"
#endif /*HAS_CALENDAR*/

#ifdef HAS_CLI
#include "cli_manager.h"
#include "log.h"
#endif /*HAS_CLI*/

#ifdef HAS_GPIO_PWM
#include "gpio_pwm_drv.h"
#endif /*HAS_LORA*/

#ifdef HAS_LORA
#include "lora_drv.h"
#endif /*HAS_LORA*/

#ifdef HAS_UBLOX
#include "ublox_driver.h"
#include "ubx_protocol.h"
#endif /*HAS_UBLOX*/

#ifdef HAS_RTCM3
#include "Rtcm3_protocol.h"
#endif /*HAS_RTCM3*/

#ifdef HAS_TBFP
#include "tbfp_protocol.h"
#endif

#ifdef HAS_NMEA
#include "nmea_protocol.h"
#endif

#ifdef HAS_HEALTH_MONITOR
#include "health_monitor.h"
#endif /*HAS_HEALTH_MONITOR*/

#ifdef HAS_FLASH_FS
#include "flash_fs.h"
#endif /*HAS_FLASH_FS*/

#ifdef HAS_PARAM
#include "param_ids.h"
#endif /*HAS_PARAM*/

#ifdef HAS_PWR_MUX
#include "pwr_mux_drv.h"
#endif /*HAS_PWR_MUX*/

#ifdef HAS_ZED_F9P
#include "zed_f9p_drv.h"
#endif /*HAS_ZED_F9P*/

bool sw_init(void) {
    bool res = true;

#ifdef HAS_LED
    res = try_init(led_init(), "LED") && res;
#endif /*HAS_LED*/

#ifdef HAS_CLI
    set_log_level(ALL_FACILITY, LOG_LEVEL_INFO);
#ifdef HAS_DEBUG
    // set_log_level(ALL_FACILITY, LOG_LEVEL_DEBUG);
#endif /*HAS_DEBUG*/

#ifdef HAS_RELEASE
    set_log_level(ALL_FACILITY, LOG_LEVEL_INFO);
#endif /*HAS_RELEASE*/
    res = try_init(cli_init(), "CLI") && res;
#endif /*HAS_CLI*/

#ifdef HAS_NMEA
    res = try_init(nmea_init(), "NMEA") && res;
#endif

#ifdef HAS_TBFP
    res = try_init(tbfp_protocol_init(&TbfpProtocol[IF_LORA], IF_LORA), "TbfpLoRa") && res;
    res = try_init(tbfp_protocol_init(&TbfpProtocol[IF_RS232], IF_RS232), "TbfpRS232") && res;
#endif /*HAS_TBFP*/

#ifdef HAS_UBLOX
    res = try_init(ublox_protocol_init(), "UBXProto") && res;
    res = try_init(ubx_driver_init(), "UBXdrv") && res;
#endif /*HAS_UBLOX*/

#ifdef HAS_RTCM3
    res = try_init(rtcm3_protocol_init(&Rtcm3Protocol[IF_UART1], IF_UART1, true), "Rtcm3UART1") && res;
    res = try_init(rtcm3_protocol_init(&Rtcm3Protocol[IF_LORA], IF_LORA, false), "Rtcm3LORA") && res;
    res = try_init(rtcm3_protocol_init(&Rtcm3Protocol[IF_RS232], IF_RS232, false), "Rtcm3RS232") && res;
#endif /*HAS_RTCM3*/

#ifdef HAS_HEALTH_MONITOR
    res = try_init(health_monotor_init(), "HM") && res;
#endif /*HAS_HEALTH_MONITOR*/

#ifdef HAS_FLASH_FS
    res = try_init(flash_fs_init(), "Flash_FS") && res;
#endif /*HAS_FLASH_FS*/

#ifdef HAS_PARAM
    res = try_init(param_init(), "param") && res; //
#endif                                            /*HAS_PARAM*/

#ifdef HAS_PWR_MUX
    res = try_init(pwr_src_init(), "PWR_MUX") && res;
#endif /*HAS_PWR_MUX*/

#ifdef HAS_EXT_RAM_EMUL
    res = try_init(ext_ram_init(), "ext_ram_em") && res;
#endif /*HAS_EXT_RAM_EMUL*/

#ifdef HAS_TASKS
    res = try_init(task_init(), "task") && res;
#endif /*TASKS*/

#ifdef HAS_BOOTLOADER
    res = try_init(boot_init(), "boot") && res;
#endif /*HAS_BOOTLOADER*/

#ifdef HAS_LORA
    res = try_init(lora_init(), "LoRa") && res;
#endif /*HAS_LORA*/

#ifdef HAS_GPIO_PWM
    res = try_init(gpio_pwm_init(), "GPIO_PWM") && res;
#endif /*HAS_LORA*/

#ifdef HAS_ZED_F9P
    res = try_init(zed_f9p_init(), "zedf9p") && res;
#endif /*HAS_ZED_F9P*/

#ifdef HAS_CALENDAR
    res = try_init(calendar_init(), "calendar") && res;
#endif /*HAS_CALENDAR*/

    return res;
}
