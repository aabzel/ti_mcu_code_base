#include "tcan4550_drv.h"

#include <stdbool.h>
#include <stdint.h>
#include <gpio.h>

#include "sys_config.h"

bool tcan4550_init(void) {
    bool out_res = true;
    GPIO_writeDio(DIO_SS1_CAN, 1);
    GPIO_writeDio(DIO_CAN_RST, 1);
    return out_res;
}

