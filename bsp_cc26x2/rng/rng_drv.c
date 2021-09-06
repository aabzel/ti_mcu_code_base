#include "rng_drv.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <ti/drivers/TRNG.h>
#include <ti/drivers/trng/TRNGCC26XX.h>

#include "debug_info.h"

#define TRNG_INSTANCE 0

TRNGCC26XX_Object trngCC26XXObjects[BOARD_TRNGCOUNT];

const TRNGCC26XX_HWAttrs trngCC26X2HWAttrs[BOARD_TRNGCOUNT] = {{
    .intPriority = ~0,
    .swiPriority = 0,
    .samplesPerCycle = 240000,
}};

const TRNG_Config TRNG_config[BOARD_TRNGCOUNT] = {
    {.object = &trngCC26XXObjects[BOARD_TRNG0], .hwAttrs = &trngCC26X2HWAttrs[BOARD_TRNG0]},
    {.object = &trngCC26XXObjects[BOARD_TRNG1], .hwAttrs = &trngCC26X2HWAttrs[BOARD_TRNG1]},
};

const uint_least8_t TRNG_count = BOARD_TRNGCOUNT;

TRNG_Handle trgHandle;
TRNG_Params param;

bool rng_init(void) {
    bool res = false;
    TRNG_init();
    TRNG_Params_init(&param);
    trgHandle = TRNG_open(TRNG_INSTANCE, &param);
    if(trgHandle) {
        res = true;
    }
    return res;
}

bool rng_read(uint32_t* out_val) {
    bool res = false;
    U32_bit_t u_val32;
    u_val32.u32 = 0;
    int_fast16_t result;
    result = TRNG_getRandomBytes(trgHandle, &u_val32.u8[0], sizeof(U32_bit_t));
    if(result != TRNG_STATUS_SUCCESS) {
        res = false;
    } else {
        *out_val = u_val32.u32;
        res = true;
    }
    return res;
}

uint32_t generate_rand_uint32(void) {
    uint32_t val = 0;
    rng_read(&val);
    return val;
}
