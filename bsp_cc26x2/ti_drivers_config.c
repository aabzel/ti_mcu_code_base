/*
 *  ======== ti_drivers_config.c ========
 *  Configured TI-Drivers module definitions
 *
 *  DO NOT EDIT - This file is generated for the CC26X2R1_LAUNCHXL
 *  by the SysConfig tool.
 */

#include <stddef.h>
#include <stdint.h>

#ifndef DeviceFamily_CC26X2
#define DeviceFamily_CC26X2
#endif

#include <ti/devices/DeviceFamily.h>
#include "board_layout.h"
#include "ti_drivers_config.h"

/*
 *  =============================== AESCCM ===============================
 */

#include <ti/drivers/AESCCM.h>
#include <ti/drivers/aesccm/AESCCMCC26XX.h>

#define CONFIG_AESCCM_COUNT 1
AESCCMCC26XX_Object aesccmCC26XXObjects[CONFIG_AESCCM_COUNT];

/*
 *  ======== aesccmCC26XXHWAttrs ========
 */
const AESCCMCC26XX_HWAttrs aesccmCC26XXHWAttrs[CONFIG_AESCCM_COUNT] = {
    {
        .intPriority = (~0),
    },
};

const AESCCM_Config AESCCM_config[CONFIG_AESCCM_COUNT] = {
    {   /* CONFIG_AESCCM0 */
        .object  = &aesccmCC26XXObjects[CONFIG_AESCCM0],
        .hwAttrs = &aesccmCC26XXHWAttrs[CONFIG_AESCCM0]
    },
};

const uint_least8_t CONFIG_AESCCM0_CONST = CONFIG_AESCCM0;
const uint_least8_t AESCCM_count = CONFIG_AESCCM_COUNT;

/*
 *  =============================== AESCTR ===============================
 */

#include <ti/drivers/AESCTR.h>
#include <ti/drivers/aesctr/AESCTRCC26XX.h>

#define CONFIG_AESCTR_COUNT 1
AESCTRCC26XX_Object aesctrCC26XXObjects[CONFIG_AESCTR_COUNT];

/*
 *  ======== aesctrCC26XXHWAttrs ========
 */
const AESCTRCC26XX_HWAttrs aesctrCC26XXHWAttrs[CONFIG_AESCTR_COUNT] = {
    {
        .intPriority = (~0),
    },
};

const AESCTR_Config AESCTR_config[CONFIG_AESCTR_COUNT] = {
    {   /* CONFIG_AESCTR_0 */
        .object  = &aesctrCC26XXObjects[CONFIG_AESCTR_0],
        .hwAttrs = &aesctrCC26XXHWAttrs[CONFIG_AESCTR_0]
    },
};

const uint_least8_t CONFIG_AESCTR_0_CONST = CONFIG_AESCTR_0;
const uint_least8_t AESCTR_count = CONFIG_AESCTR_COUNT;

/*
 *  =============================== AESCTRDRBG ===============================
 */

#include <ti/drivers/AESCTRDRBG.h>
#include <ti/drivers/aesctrdrbg/AESCTRDRBGXX.h>

#define CONFIG_AESCTRDRBG_COUNT 1

/*
 *  ======== aesctrdrbgXXObjects ========
 */
AESCTRDRBGXX_Object aesctrdrbgXXObjects[CONFIG_AESCTRDRBG_COUNT];

/*
 *  ======== aesctrdrbgXXHWAttrs ========
 */
const AESCTRDRBGXX_HWAttrs aesctrdrbgXXHWAttrs[CONFIG_AESCTRDRBG_COUNT] = {
    /* CONFIG_AESCTRDRBG_0 */
    {
        .aesctrIndex = CONFIG_AESCTR_0
    },
};

/*
 *  ======== AESCTRDRBG_config ========
 */
const AESCTRDRBG_Config AESCTRDRBG_config[CONFIG_AESCTRDRBG_COUNT] = {
    /* CONFIG_AESCTRDRBG_0 */
    {
        .object = &aesctrdrbgXXObjects[CONFIG_AESCTRDRBG_0],
        .hwAttrs = &aesctrdrbgXXHWAttrs[CONFIG_AESCTRDRBG_0]
    },
};

const uint_least8_t CONFIG_AESCTRDRBG_0_CONST = CONFIG_AESCTRDRBG_0;
const uint_least8_t AESCTRDRBG_count = CONFIG_AESCTRDRBG_COUNT;

/*
 *  =============================== AESECB ===============================
 */

#include <ti/drivers/AESECB.h>
#include <ti/drivers/aesecb/AESECBCC26XX.h>

#define CONFIG_AESECB_COUNT 1
AESECBCC26XX_Object aesecbCC26XXObjects[CONFIG_AESECB_COUNT];

/*
 *  ======== aesecbCC26XXHWAttrs ========
 */
const AESECBCC26XX_HWAttrs aesecbCC26XXHWAttrs[CONFIG_AESECB_COUNT] = {
    {
        .intPriority = (~0),
    },
};

const AESECB_Config AESECB_config[CONFIG_AESECB_COUNT] = {
    {   /* CONFIG_AESECB0 */
        .object  = &aesecbCC26XXObjects[CONFIG_AESECB0],
        .hwAttrs = &aesecbCC26XXHWAttrs[CONFIG_AESECB0]
    },
};


const uint_least8_t CONFIG_AESECB0_CONST = CONFIG_AESECB0;
const uint_least8_t AESECB_count = CONFIG_AESECB_COUNT;

/*
 *  =============================== DMA ===============================
 */

#include <ti/drivers/dma/UDMACC26XX.h>
#include <ti/devices/cc13x2_cc26x2/driverlib/udma.h>
#include <ti/devices/cc13x2_cc26x2/inc/hw_memmap.h>

UDMACC26XX_Object udmaCC26XXObject;

const UDMACC26XX_HWAttrs udmaCC26XXHWAttrs = {
    .baseAddr        = UDMA0_BASE,
    .powerMngrId     = PowerCC26XX_PERIPH_UDMA,
    .intNum          = INT_DMA_ERR,
    .intPriority     = (~0)
};

const UDMACC26XX_Config UDMACC26XX_config[1] = {
    {
        .object         = &udmaCC26XXObject,
        .hwAttrs        = &udmaCC26XXHWAttrs,
    },
};

/*
 *  =============================== ECDH ===============================
 */

#include <ti/drivers/ECDH.h>
#include <ti/drivers/ecdh/ECDHCC26X2.h>

#define CONFIG_ECDH_COUNT 1

ECDHCC26X2_Object ecdhCC26X2Objects[CONFIG_ECDH_COUNT];

/*
 *  ======== ecdhCC26X2HWAttrs ========
 */
const ECDHCC26X2_HWAttrs ecdhCC26X2HWAttrs[CONFIG_ECDH_COUNT] = {
    {
        .intPriority = (~0),
    },
};

const ECDH_Config ECDH_config[CONFIG_ECDH_COUNT] = {
    {   /* CONFIG_ECDH0 */
        .object         = &ecdhCC26X2Objects[CONFIG_ECDH0],
        .hwAttrs        = &ecdhCC26X2HWAttrs[CONFIG_ECDH0]
    },
};

const uint_least8_t CONFIG_ECDH0_CONST = CONFIG_ECDH0;
const uint_least8_t ECDH_count = CONFIG_ECDH_COUNT;

/*
 *  =============================== GPIO ===============================
 */

#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOCC26XX.h>

/* The range of pins available on this device */
const uint_least8_t GPIO_pinLowerBound = 0;
const uint_least8_t GPIO_pinUpperBound = 30;

/*
/*
 *  ======== gpioUserArgs ========
 *  Array of user argument pointers
 *  Change at runtime with GPIO_setUserArg()
 *  Get values with GPIO_getUserArg()
 */

void* gpioUserArgs[31];
const GPIO_Config GPIO_config = {
    .configs = (GPIO_PinConfig *)gpioPinConfigs,
    .callbacks = (GPIO_CallbackFxn *)gpioCallbackFunctions,
    .userArgs = gpioUserArgs,
    .intPriority = (~0)
};

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26X2.h>
#include "ti_drivers_config.h"

extern void PowerCC26XX_standbyPolicy(void);
extern bool PowerCC26XX_calibrate(unsigned int);

const PowerCC26X2_Config PowerCC26X2_config = {
    .enablePolicy             = true,
    .policyInitFxn            = NULL,
    .policyFxn                = PowerCC26XX_standbyPolicy,
    .calibrateFxn             = PowerCC26XX_calibrate,
    .calibrateRCOSC_LF        = true,
    .calibrateRCOSC_HF        = true,
    .enableTCXOFxn            = NULL
};


/*
 *  =============================== RF Driver ===============================
 */
#include <ti/drivers/GPIO.h>
#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/ioc.h)
#include <ti/drivers/rf/RF.h>

/*
 * Platform-specific driver configuration
 */
const RFCC26XX_HWAttrsV2 RFCC26XX_hwAttrs = {
    .hwiPriority        = (~0),
    .swiPriority        = (uint8_t)0,
    .xoscHfAlwaysNeeded = true,
    .globalCallback     = NULL,
    .globalEventMask    = 0
};


/*
 *  =============================== TRNG ===============================
 */

#include <ti/drivers/TRNG.h>
#include <ti/drivers/trng/TRNGCC26XX.h>

#define CONFIG_TRNG_COUNT 1

TRNGCC26XX_Object trngCC26XXObjects[CONFIG_TRNG_COUNT];

/*
 *  ======== trngCC26XXHWAttrs ========
 */
static const TRNGCC26XX_HWAttrs trngCC26XXHWAttrs[CONFIG_TRNG_COUNT] = {
    {
        .intPriority = (~0),
        .swiPriority = 0,
        .samplesPerCycle = 240000
    },
};

const TRNG_Config TRNG_config[CONFIG_TRNG_COUNT] = {
    {   /* CONFIG_TRNG_0 */
        .object         = &trngCC26XXObjects[CONFIG_TRNG_0],
        .hwAttrs        = &trngCC26XXHWAttrs[CONFIG_TRNG_0]
    },
};

const uint_least8_t CONFIG_TRNG_0_CONST = CONFIG_TRNG_0;
const uint_least8_t TRNG_count = CONFIG_TRNG_COUNT;

#include <stdbool.h>

#include <ti/devices/cc13x2_cc26x2/driverlib/ioc.h>
#include <ti/devices/cc13x2_cc26x2/driverlib/cpu.h>

#include <ti/drivers/GPIO.h>

/* Board GPIO defines */
#define BOARD_EXT_FLASH_SPI_CS      20
#define BOARD_EXT_FLASH_SPI_CLK     10
#define BOARD_EXT_FLASH_SPI_MOSI    9
#define BOARD_EXT_FLASH_SPI_MISO    8


/*
 *  ======== Board_sendExtFlashByte ========
 */
void Board_sendExtFlashByte(uint8_t byte)
{
    uint8_t i;

    /* SPI Flash CS */
    GPIO_write(BOARD_EXT_FLASH_SPI_CS, 0);

    for (i = 0; i < 8; i++) {
        GPIO_write(BOARD_EXT_FLASH_SPI_CLK, 0); /* SPI Flash CLK */

        /* SPI Flash MOSI */
        GPIO_write(BOARD_EXT_FLASH_SPI_MOSI, (byte >> (7 - i)) & 0x01);
        GPIO_write(BOARD_EXT_FLASH_SPI_CLK, 1);  /* SPI Flash CLK */

        /*
         * Waste a few cycles to keep the CLK high for at
         * least 45% of the period.
         * 3 cycles per loop: 8 loops @ 48 Mhz = 0.5 us.
         */
        CPUdelay(8);
    }

    GPIO_write(BOARD_EXT_FLASH_SPI_CLK, 0);  /* CLK */
    GPIO_write(BOARD_EXT_FLASH_SPI_CS, 1);  /* CS */

    /*
     * Keep CS high at least 40 us
     * 3 cycles per loop: 700 loops @ 48 Mhz ~= 44 us
     */
    CPUdelay(700);
}

/*
 *  ======== Board_wakeUpExtFlash ========
 */
void Board_wakeUpExtFlash(void)
{
    /* SPI Flash CS*/
    GPIO_setConfig(BOARD_EXT_FLASH_SPI_CS, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_HIGH | GPIO_CFG_OUT_STR_MED);

    /*
     *  To wake up we need to toggle the chip select at
     *  least 20 ns and ten wait at least 35 us.
     */

    /* Toggle chip select for ~20ns to wake ext. flash */
    GPIO_write(BOARD_EXT_FLASH_SPI_CS, 0);
    /* 3 cycles per loop: 1 loop @ 48 Mhz ~= 62 ns */
    CPUdelay(1);
    GPIO_write(BOARD_EXT_FLASH_SPI_CS, 1);
    /* 3 cycles per loop: 560 loops @ 48 Mhz ~= 35 us */
    CPUdelay(560);
}



#include <ti/drivers/Board.h>

/*
 *  ======== Board_initHook ========
 *  Perform any board-specific initialization needed at startup.  This
 *  function is declared weak to allow applications to override it if needed.
 */
void __attribute__((weak)) Board_initHook(void)
{
}

/*
 *  ======== Board_init ========
 *  Perform any initialization needed before using any board APIs
 */
void Board_init(void)
{
    /* ==== /ti/drivers/Power initialization ==== */
    Power_init();

    /* ==== /ti/devices/CCFGTemplateInit initialization ==== */

    /* ==== /ti/drivers/RF initialization ==== */
    /* ==== /ti/drivers/GPIO initialization ==== */
    /* Setup GPIO module and default-initialise pins */
    GPIO_init();

    //Board_shutDownExtFlash();

    Board_initHook();
}

