#include <stdio.h>

#include "base_cmd.h"
#include "common_commands.h"
#include "log.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAS_LED
#include "led_commands.h"
#else
#define LED_COMMANDS
#endif

#ifdef HAS_EXT_RAM_EMUL
#include "ext_ram_sim_commands.h"
#else
#define EXT_RAM_EMUL_COMMANDS
#endif

#ifdef HAS_MX25L6433
#include "mx25l6433f_commands.h"
#else
#define MX25L6433_COMMANDS
#endif

#ifdef HAS_CORTEX_M4
#include "core_commands.h"
#else
#define CORTEX_M4_COMMANDS
#endif

#ifdef HAS_UBLOX
#include "ublox_command.h"
#else
#define UBLOX_COMMANDS
#endif

#ifdef HAS_NMEA
#include "nmea_command.h"
#else
#define NMEA_COMMANDS
#endif


#ifdef HAS_BMP180
#include "bmp180_commands.h"
#else
#define BMP180_COMMANDS
#endif

#ifdef HAS_BQ25171_Q1
#include "bq25171_q1_commands.h"
#else
#define BQ25171_Q1_COMMANDS
#endif /*HAS_BQ25171_Q1*/

#ifdef HAS_FLASH_FS
#include "flash_fs_commands.h"
#else
#define FLASH_FS_COMMANDS
#endif

#ifdef HAS_FLASH_NVS
#include "flash_nvs_commands.h"
#else
#define FLASH_NVS_COMMANDS
#endif

#ifdef HAS_HEALTH_MONITOR
#include "health_monitor_command.h"
#else
#define HEALTH_MONITOR_COMMANDS
#endif

#ifdef HAS_LORA
#include "lora_commands.h"
#else
#define LORA_COMMANDS
#endif

#ifdef HAS_RF
#include "rf_commands.h"
#else
#define RF_COMMANDS
#endif /*HAS_RF*/

#ifdef HAS_SX1262
#include "sx1262_commands.h"
#else
#define SX1262_COMMANDS
#endif


#ifdef HAS_PARAM
#include "param_commands.h"
#else
#define PARAM_COMMANDS
#endif

#ifdef HAS_PWR_MUX
#include "pwr_mux_commands.h"
#else
#define PWR_MUX_COMMANDS
#endif

#ifdef HAS_RNG
#include "rng_commands.h"
#else
#define TRNG_COMMANDS
#endif



#ifdef HAS_TBFP
#include "tbfp_commands.h"
#else
#define TBFP_COMMANDS
#endif

#ifdef HAS_TELEMATIC
#include "telematic_commands.h"
#else
#define TELEMATIC_COMMANDS
#endif




#ifdef HAS_RTCM3
#include "rtcm3_commands.h"
#else
#define RTCM3_COMMANDS
#endif

#ifdef HAS_ZED_F9P
#include "zed_f9p_commands.h"
#else
#define ZED_F9P_COMMANDS
#endif

#define SHELL_COMMANDS                                                                                                 \
    BMP180_COMMANDS                                                                                                    \
    BQ25171_Q1_COMMANDS                                                                                                \
    COMMON_COMMANDS                                                                                                    \
    CORTEX_M4_COMMANDS                                                                                                 \
    EXT_RAM_EMUL_COMMANDS                                                                                              \
    FLASH_FS_COMMANDS                                                                                                  \
    FLASH_NVS_COMMANDS                                                                                                 \
    HEALTH_MONITOR_COMMANDS                                                                                            \
    TELEMATIC_COMMANDS                                                 \
    TBFP_COMMANDS \
    LED_COMMANDS                                                                                                       \
    LOG_COMMANDS                                                                                                       \
    LORA_COMMANDS                                                                                                      \
    MX25L6433_COMMANDS                                                                                                 \
    NMEA_COMMANDS                                                                                                      \
    PARAM_COMMANDS                                                                                                     \
    PWR_MUX_COMMANDS                                                                                                   \
    RF_COMMANDS                                                                                                        \
    RTCM3_COMMANDS    \
    SX1262_COMMANDS                                                                                                    \
    TRNG_COMMANDS                                                                                                      \
    UBLOX_COMMANDS                 \
    ZED_F9P_COMMANDS


#ifdef __cplusplus
} /* extern "C" */
#endif
