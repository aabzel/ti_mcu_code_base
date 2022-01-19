#include "test_spi.h"

#include <string.h>

#include "gpio_drv.h"
#include "spi_drv.h"
#include "sys_config.h"
#include "unit_test_check.h"

bool test_spi_write(void) {
    uint8_t array[2];
    gpio_set_state(DIO_SX1262_SS, 1);
#ifdef HAS_CAN
    gpio_set_state(DIO_CAN_SS, 1);
#endif
    uint32_t init_int_cnt = SpiInstance[0].it_cnt;
    memset(array,0xFF,sizeof(array));
    EXPECT_TRUE( spi_write(SPI0_INX, array, sizeof(array)));
    EXPECT_GR(init_int_cnt, SpiInstance[0].it_cnt);

    return true;
}

#ifdef HAS_SPI_WAIT_WRITE
bool test_spi_wait_write(void) {
    uint8_t array[2];
    gpio_set_state(DIO_SX1262_SS, 1);
#ifdef HAS_CAN
    gpio_set_state(DIO_CAN_SS, 1);
#endif
    memset(array,0xFF,sizeof(array));
    EXPECT_TRUE( spi_wait_write(SPI0_INX, array, sizeof(array)));
    EXPECT_TRUE(  spi_wait_tx_done(SPI0_INX));
    return true;
}
#endif

bool test_spi_read(void) {
    uint8_t array[2];
    gpio_set_state(DIO_SX1262_SS, 1);
#ifdef HAS_CAN
    gpio_set_state(DIO_CAN_SS, 1);
#endif
    uint32_t init_int_cnt = SpiInstance[0].it_cnt;
    memset(array,0xFF,sizeof(array));
    EXPECT_TRUE( spi_read(SPI0_INX, array, sizeof(array)));
    EXPECT_GR(init_int_cnt, SpiInstance[0].it_cnt);

    return true;
}
