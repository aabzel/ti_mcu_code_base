#include "tcan4550_drv.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <gpio.h>

#include "spi_drv.h"
#include "clocks.h"
#include "debug_info.h"
#include "sys_config.h"

bool tcan4550_init(void) {
    bool res = true;
    GPIO_writeDio(DIO_CAN_SS, 1);
    GPIO_writeDio(DIO_CAN_RST, 0);

    res=tcan4550_reset();
    return res;
}

bool tcan4550_reset(void){
    bool res= true;
    GPIO_writeDio(DIO_CAN_RST, 1);
    /*tPULSE_WIDTH Width of the input pulse 30 us*/
    delay_ms(1);
    GPIO_writeDio(DIO_CAN_RST, 0);
    return res;
}

bool tcan4550_chip_select(bool state) {
    bool res = false;
    if(true == state) {
        GPIO_writeDio(DIO_SX1262_SS, 1);
        GPIO_writeDio(DIO_CAN_SS, 0);
        res = true;
    } else if(false == state) {
        GPIO_writeDio(DIO_SX1262_SS, 1);
        GPIO_writeDio(DIO_CAN_SS, 1);
        res = true;
    } else {
        res = false;
    }
    return res;
}

bool tcan4550_read(uint16_t address, uint8_t len, uint8_t *out_array, uint32_t size){
    bool res = true;
    if((NULL!=out_array) &&((len*4)<size)){
        res = true;
        res = tcan4550_chip_select(true) && res;
        delay_ms(1);
        uint8_t tx_array[4];
        tx_array[0] = READ_OP_CODE;
        memcpy(&tx_array[1], &address, 2);
        tx_array[3] = len;
        res= spi_write(SPI0_INX , tx_array, sizeof(tx_array))&&res;
        res= spi_read(SPI0_INX , out_array, (uint16_t) (len*4))&&res;
        delay_ms(1);
        res = tcan4550_chip_select(false)&&res;
    } else {
        res = false;
    }
    return res;
}
