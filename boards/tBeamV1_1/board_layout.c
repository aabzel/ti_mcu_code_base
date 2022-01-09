#include "board_layout.h"

#include <stdlib.h>
#include <ti/drivers/gpio/GPIOCC26XX.h>

#include "gpio_drv.h"
#include "sys_config.h"

/*line order does not  matters!*/
const Pin_t PinTable[DIO_CNT] = {
};

/*line order matters!*/
GPIO_PinConfig gpioPinConfigs[GPIO_COUNT] = {
};

/*line order matters!*/
PIN_Config BoardGpioInitTable[GPIO_COUNT + 1] = {
         PIN_TERMINATE
};

/*line order matters!*/
GPIO_CallbackFxn gpioCallbackFunctions[GPIO_COUNT] = {
};
