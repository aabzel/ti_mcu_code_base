#include "board_layout.h"

#include <stdlib.h>

#include "gpio_drv.h"

/*
    uint8_t gpio_pin;
	char gpio_port;
	char mcu_port;
    uint8_t mcu_pin;
    char name[PIN_NAME_LEN];
*/	
const Pin_t PinTable[DIO_CNT] = {
	{7,'E','M',7,"UART7_Rx"}
};



