#include <stdio.h>

#include "base_cmd.h"
#include "common_commands.h"
#include "log.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAS_GNSS
#include "gnss_commands.h"
#else
#define GNSS_COMMANDS
#endif

#ifdef HAS_AXP192_COMMANDS
#include "axp192_commands.h"
#else
#define AXP192_COMMANDS
#endif

#ifdef HAS_LED_COMMANDS
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

#ifdef HAS_CORTEX_M4_COMMANDS
#include "core_commands.h"
#else
#define CORTEX_M4_COMMANDS
#endif

#ifdef HAS_BMP180
#include "bmp180_commands.h"
#else
#define BMP180_COMMANDS
#endif

#ifdef HAS_BQ25171_Q1_COMMANDS
#include "bq25171_q1_commands.h"
#else
#define BQ25171_Q1_COMMANDS
#endif /*HAS_BQ25171_Q1*/

#ifdef HAS_FLASH_FS_COMMANDS
#include "flash_fs_commands.h"
#else
#define FLASH_FS_COMMANDS
#endif

#ifdef HAS_FLASH_NVS_COMMANDS
#include "flash_nvs_commands.h"
#else
#define FLASH_NVS_COMMANDS
#endif

#ifdef HAS_HEALTH_MONITOR_COMMANDS
#include "health_monitor_command.h"
#else
#define HEALTH_MONITOR_COMMANDS
#endif

#ifdef HAS_LORA_COMMANDS
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

#ifdef HAS_PARAM_COMMANDS
#include "param_commands.h"
#else
#define PARAM_COMMANDS
#endif

#ifdef HAS_PWR_MUX_COMMANDS
#include "pwr_mux_commands.h"
#else
#define PWR_MUX_COMMANDS
#endif

#ifdef HAS_RNG
#include "rng_commands.h"
#else
#define TRNG_COMMANDS
#endif

#ifdef HAS_SYSTEM_COMMANDS
#include "system_commands.h"
#else
#define SYSTEM_COMMANDS
#endif

#ifdef HAS_TBFP_COMMANDS
#include "tbfp_commands.h"
#else
#define TBFP_COMMANDS
#endif

#ifdef HAS_TELEMATIC_COMMANDS
#include "telematic_commands.h"
#else
#define TELEMATIC_COMMANDS
#endif

#define SHELL_COMMANDS                                                                                                 \
    AXP192_COMMANDS                                                                                                    \
    BMP180_COMMANDS                                                                                                    \
    BQ25171_Q1_COMMANDS                                                                                                \
    COMMON_COMMANDS                                                                                                    \
    CORTEX_M4_COMMANDS                                                                                                 \
    EXT_RAM_EMUL_COMMANDS                                                                                              \
    FLASH_FS_COMMANDS                                                                                                  \
    FLASH_NVS_COMMANDS                                                                                                 \
    GNSS_COMMANDS                                                                                                      \
    HEALTH_MONITOR_COMMANDS                                                                                            \
    LED_COMMANDS                                                                                                       \
    LOG_COMMANDS                                                                                                       \
    LORA_COMMANDS                                                                                                      \
    MX25L6433_COMMANDS                                                                                                 \
    PARAM_COMMANDS                                                                                                     \
    PWR_MUX_COMMANDS                                                                                                   \
    RF_COMMANDS                                                                                                        \
    SX1262_COMMANDS                                                                                                    \
    SYSTEM_COMMANDS                                                                                                    \
    TBFP_COMMANDS                                                                                                      \
    TELEMATIC_COMMANDS                                                                                                 \
    TRNG_COMMANDS

#ifdef __cplusplus
} /* extern "C" */
#endif
