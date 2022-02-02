#include "flash_nvs_drv.h"

#include <flash.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <hw_ccfg.h>
#include <hw_memmap.h>
#include <hw_types.h>
#include <ti/drivers/NVS.h>
#include <ti/drivers/nvs/NVSCC26XX.h>
#include <vims.h>
#ifndef HAS_LOG
#include "log.h"
#endif
#include "bit_utils.h"
#include "data_utils.h"
#include "memory_layout.h"
#include "none_blocking_pause.h"

#ifndef HAS_FLASH_NVS
#error "Define FLASH_NVS!"
#endif

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

bool flash_nvs_init(void) {
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

bool flash_nvs_write(uint32_t flas_addr, uint8_t* array, uint32_t array_len) {
    bool res = false;
    int_fast16_t ret;
    if((NVS_FLASH_START <= flas_addr) && (flas_addr < (NVS_FLASH_START + NVS_SIZE))) {
        size_t offset = flas_addr - NVS_FLASH_START;
        NVS_unlock(nvsHandle);// Error Here
        ret = NVS_write(nvsHandle, offset, (void*)array, (size_t)array_len, NVS_WRITE_POST_VERIFY);
        if(NVS_STATUS_SUCCESS == ret) {// Error -1
            res = true;
        }else{
#ifndef HAS_LOG
            LOG_ERROR(NVS,"WriteErr %d", ret);
#endif
        }
        NVS_lock(nvsHandle, 0);
    }
    return res;
}

bool flash_nvs_erase(uint32_t addr, uint32_t array_len) {
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


