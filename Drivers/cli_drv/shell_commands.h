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

#ifdef HAS_HEALTH_MONITOR
#include "health_monitor_command.h"
#else
#define HEALTH_MONITOR_COMMANDS
#endif

#ifdef HAS_BMP180
#include "bmp180_commands.h"
#else
#define BMP180_COMMANDS
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
#endif

#ifdef HAS_SX1262
#include "sx1262_commands.h"
#else
#define SX1262_COMMANDS
#endif


#ifdef HAS_FLASH_FS
#include "flash_fs_commands.h"
#else
#define FLASH_FS_COMMANDS
#endif

#ifdef HAS_PARAM
#include "param_commands.h"
#else
#define PARAM_COMMANDS
#endif

#ifdef HAS_RNG
#include "rng_commands.h"
#else
#define TRNG_COMMANDS
#endif

#define SHELL_COMMANDS                                                                                                 \
    BMP180_COMMANDS                                                                                                    \
    COMMON_COMMANDS                                                                                                    \
    CORTEX_M4_COMMANDS                                                                                                 \
    FLASH_FS_COMMANDS                                                                                                  \
    LED_COMMANDS                                                                                                       \
    LORA_COMMANDS                                                                                                      \
    LOG_COMMANDS                                                                              \
    PARAM_COMMANDS \
    HEALTH_MONITOR_COMMANDS                                                                                            \
    NMEA_COMMANDS                                                                                                      \
    RF_COMMANDS                                                                                                        \
    SX1262_COMMANDS                                                                                                    \
    TRNG_COMMANDS                                                              \
    UBLOX_COMMANDS

#ifdef __cplusplus
} /* extern "C" */
#endif
