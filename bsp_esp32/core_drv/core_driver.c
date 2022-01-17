#include "core_driver.h"

#include <stdio.h>
#include <string.h>

#include "array.h"
#include "bit_utils.h"
#include "boot_cfg.h"
#include "data_utils.h"
#include "read_mem.h"
#include "sys.h"
#include "esp_system.h"


uint32_t cpu_get_id(void) {
    uint32_t value=0;

    return value;
}

uint64_t get_ble_mac(void) {
    Type64Union_t uValue;
    uValue.u64 = 0;
    return uValue.u64;
}

bool reboot(void) {
    bool res = false;
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
    return res;
}
