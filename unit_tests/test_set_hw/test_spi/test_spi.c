#include "test_spi.h"

#include <string.h>
#include <gpio.h>

#include "spi_drv.h"
#include "sys_config.h"
#include "unit_test_check.h"

bool test_spi_write(void) {
    uint8_t array[2];
    GPIO_writeDio(DIO_SX1262_SS, 1);
    GPIO_writeDio(DIO_SS1_CAN, 1);
    uint32_t init_int_cnt = SpiInstance[0].it_cnt;
    memset(array,0xFF,sizeof(array));
    EXPECT_TRUE( spi_write(SPI0_INX, array, sizeof(array)));
    EXPECT_GR(init_int_cnt, SpiInstance[0].it_cnt,0);

    return true;
}

bool test_spi_read(void) {
    uint8_t array[2];
    GPIO_writeDio(DIO_SX1262_SS, 1);
    GPIO_writeDio(DIO_SS1_CAN, 1);
    uint32_t init_int_cnt = SpiInstance[0].it_cnt;
    memset(array,0xFF,sizeof(array));
    EXPECT_TRUE( spi_read(SPI0_INX, array, sizeof(array)));
    EXPECT_GR(init_int_cnt, SpiInstance[0].it_cnt,0);

    return true;
}
