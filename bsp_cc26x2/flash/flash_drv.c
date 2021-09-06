#include "flash_drv.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <ti/drivers/NVS.h>
#include <ti/drivers/nvs/NVSCC26XX.h>

#include "none_blocking_pause.h"

uint8_t nvs_buffer[NVS_BUFF_SIZE];
NVS_Handle nvsHandle;
NVS_Attrs regionAttrs;
static NVS_Params nvsParams;

NVSCC26XX_Object nvsCC26XXObjects[1];

/*
 *  NVSCC26XX Internal NVS flash region definitions
 *
 * Place uninitialized char arrays at addresses
 * corresponding to the 'regionBase' addresses defined in
 * the configured NVS regions. These arrays are used as
 * place holders so that the linker will not place other
 * content there.
 *
 * For GCC targets, the char arrays are each placed into
 * the shared ".nvs" section. The user must add content to
 * their GCC linker command file to place the .nvs section
 * at the lowest 'regionBase' address specified in their NVS
 * regions.
 */

static char flashBuf0[NVS_SIZE] __attribute__((retain, noinit, location(NVS_FLASH_START)));

static const NVSCC26XX_HWAttrs nvsCC26XXHWAttrs[1] = {
    /* CONFIG_NVSINTERNAL */
    {.regionBase = (void*)flashBuf0, .regionSize = NVS_SIZE},
};

const NVS_Config NVS_config[CONFIG_NVS_COUNT] = {
    /* CONFIG_NVSINTERNAL */
    {
        .fxnTablePtr = &NVSCC26XX_fxnTable,
        .object = &nvsCC26XXObjects[0],
        .hwAttrs = &nvsCC26XXHWAttrs[0],
    },
};

const uint_least8_t CONFIG_NVSINTERNAL_CONST = CONFIG_NVSINTERNAL;
const uint_least8_t NVS_count = CONFIG_NVS_COUNT;

bool flash_init(void) {
    bool res = true;
    NVS_init();
    NVS_Params_init(&nvsParams);
    nvsHandle = NVS_open(CONFIG_NVSINTERNAL, &nvsParams);
    if(NULL == nvsHandle) {
        res = false;
    } else {
        res = true;
        NVS_getAttrs(nvsHandle, &regionAttrs);
    }

    return res;
}

bool flash_read(uint32_t* addr, uint8_t* rx_array, uint32_t array_len) { return false; }

bool flash_scan(uint8_t* base, uint32_t size, float* usage_pec, uint32_t* spare, uint32_t* busy) {
    bool res = false;
    if(usage_pec && spare && busy) {
        res = true;
        uint8_t* addr = base;
        uint32_t cnt = 0;
        for(addr = base, cnt = 0; addr < (base + size); addr++, cnt++) {
            if(0xFF == (*addr)) {
                (*spare)++;
            } else {
                (*busy)++;
            }
            if(!(cnt % 1000)) {
                wait_in_loop_ms(5);
            }
        }
        *usage_pec = (float)(((float)(100U * (*busy))) / ((float)size));
    }
    return res;
}

bool flash_write(uint32_t flas_addr, uint8_t* array, uint32_t array_len) {
    bool res = false;
    int_fast16_t ret;
    if((NVS_FLASH_START <= flas_addr) && (flas_addr < (NVS_FLASH_START + NVS_SIZE))) {
        size_t offset = flas_addr - NVS_FLASH_START;
        NVS_unlock(nvsHandle);
        ret = NVS_write(nvsHandle, offset, (void*)array, (size_t)array_len, NVS_WRITE_POST_VERIFY);
        if(NVS_STATUS_SUCCESS == ret) {
            res = true;
        }
        NVS_lock(nvsHandle, 0);
    }
    return res;
}

bool flash_erase(uint32_t addr, uint32_t array_len) {
    bool res = false;
    if((NVS_FLASH_START <= addr) && addr < (NVS_FLASH_START + NVS_SIZE)) {
        int_fast16_t ret;
        NVS_unlock(nvsHandle);
        size_t offset = addr - NVS_FLASH_START;
        ret = NVS_erase(nvsHandle, offset, (size_t)array_len);
        if(NVS_STATUS_SUCCESS == ret) {
            res = true;
        }
        NVS_lock(nvsHandle, 0);
    }
    return res;
}

bool is_errased(uint32_t addr, uint32_t size) {
    bool res = false;
    res = is_arr_pat((uint8_t*)addr, size, 0xff);
    return res;
}
