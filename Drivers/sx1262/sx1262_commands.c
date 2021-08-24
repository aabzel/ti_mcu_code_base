#include "sx1262_commands.h"

#include <inttypes.h>

#include "convert.h"
#include "data_utils.h"
#include "debug_info.h"
#include "gpio_drv.h"
#include "io_utils.h"
#include "log.h"
#include "spi_drv.h"
#include "sx1262_drv.h"

bool sx1262_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        uint_fast8_t ret;
        ret = GPIO_read(CONFIG_GPIO_LORA_CS);
        if(0 == ret) {
            io_printf("sx1262 selected" CRLF);
        }

        ret = GPIO_read(CONFIG_GPIO_LORA_RST);
        if(0 == ret) {
            io_printf("sx1262 under reset" CRLF);
        }

        ret = GPIO_read(CONFIG_GPIO_LORA_INT);
        io_printf("INT: %u" CRLF, ret);

        ret = GPIO_read(CONFIG_GPIO_LORA_BSY);
        if(1 == ret) {
            io_printf("sx1262 busy" CRLF);
        }
    } else {
        LOG_ERROR(SPI, "Usage: sxd");
    }
    return res;
}

bool sx1262_init_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        res = sx1262_init();
        if(res) {
            LOG_INFO(SPI, "OK");
        }
    } else {
        LOG_ERROR(SPI, "Usage: sxi");
    }
    return res;
}
