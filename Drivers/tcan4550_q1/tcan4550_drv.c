#include "tcan4550_drv.h"

#include <gpio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "clocks.h"
#include "debug_info.h"
#include "log.h"
#include "spi_drv.h"
#include "sys_config.h"

const uint64_t exp_dev_id = 0x343535305443414E;

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

bool tcan4550_read(uint16_t address, uint8_t len, uint8_t* out_array, uint32_t size) {
    bool res = true;
    if((NULL != out_array) && ((len * 4) <= size)) {
        res = true;
        res = tcan4550_chip_select(true) && res;
        delay_ms(1);
        uint8_t tx_array[4];
        tx_array[0] = READ_OP_CODE;
        memcpy(&tx_array[1], &address, 2);
        tx_array[3] = len;
        res = spi_write(SPI0_INX, tx_array, sizeof(tx_array)) && res;
        res = spi_read(SPI0_INX, out_array, (uint16_t)(len * 4)) && res;
        delay_ms(1);
        res = tcan4550_chip_select(false) && res;
    } else {
        res = false;
    }
    return res;
}

bool is_tcan4550_connected(void) {
    bool res = false;
    uint64_t read_dev_id = 0;
    res = tcan4550_read(DEVICE_ID_0, 2, (uint8_t*)&read_dev_id, sizeof(read_dev_id));
    if(res) {
        if(exp_dev_id == read_dev_id) {
            res = true;
            LOG_INFO(CAN, "spot dev id: 0x%llx", read_dev_id);
            print_mem((uint8_t*)&read_dev_id, sizeof(read_dev_id), true, true);
        } else {
            res = false;
            LOG_ERROR(CAN, "Unable to spot dev id: 0x%llx", read_dev_id);
        }
    } else {
        LOG_ERROR(CAN, "Unable to read reg");
    }
    return res;
}

bool tcan4550_reset(void) {
    bool res = true;
    /*see page 25 in spec 8.3.8 RST Pin*/
    GPIO_writeDio(DIO_CAN_RST, 1);
    delay_ms(1);
    GPIO_writeDio(DIO_CAN_RST, 0);
    /*tPULSE_WIDTH Width of the input pulse 30 us*/
    delay_ms(1);
    return res;
}

bool tcan4550_init(void) {
    bool res = true;
    GPIO_writeDio(DIO_CAN_SS, 1);
    GPIO_writeDio(DIO_CAN_RST, 1);

    res = tcan4550_reset();

    res = is_tcan4550_connected();
    return res;
}
