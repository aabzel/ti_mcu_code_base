#include "sx1262_drv.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "gpio_drv.h"
#include "spi_drv.h"

static bool sx1262_init_gpio(void){
    bool res = true;
    GPIO_setConfig(CONFIG_GPIO_LORA_RST, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
    GPIO_setConfig(CONFIG_GPIO_LORA_CS, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
    GPIO_setConfig(CONFIG_GPIO_LORA_INT, GPIO_CFG_IN_NOPULL );
    GPIO_setConfig(CONFIG_GPIO_LORA_BSY, GPIO_CFG_IN_NOPULL );
    GPIO_write(CONFIG_GPIO_LORA_RST, 1);
    GPIO_write(CONFIG_GPIO_LORA_CS, 1);
    return res;
}

bool sx1262_init(void) {
	bool res = true;
	res = sx1262_init_gpio() &&res;
	return res;
}
