#include "test_spi.h"

#include <string.h>

#include "gpio_drv.h"
#include "spi_drv.h"
#include "sys_config.h"
#include "unit_test_check.h"

static bool test_spi_write_num(SpiName_t spi_num) {
    uint8_t array[2];
#ifndef ESP32
    uint32_t init_int_cnt = SpiInstance[spi_num].it_cnt;
#endif
    memset(array,0xFF,sizeof(array));
    EXPECT_TRUE( spi_write(spi_num, array, sizeof(array)));
#ifndef ESP32
    EXPECT_GR(init_int_cnt, SpiInstance[spi_num].it_cnt);
#endif
    return true;
}

#ifdef HAS_SPI0
bool test_spi0_write(void) {
	EXPECT_TRUE( test_spi_write_num((SpiName_t)0));
    return true;
}
#endif

#ifdef HAS_SPI1
bool test_spi1_write(void) {
	EXPECT_TRUE( test_spi_write_num((SpiName_t)1));
    return true;
}
#endif

#ifdef HAS_SPI2
bool test_spi2_write(void) {
	EXPECT_TRUE( test_spi_write_num((SpiName_t)2));
    return true;
}
#endif

#ifdef HAS_SPI3
bool test_spi3_write(void) {
	EXPECT_TRUE( test_spi_write_num((SpiName_t)3));
    return true;
}
#endif

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

static bool test_spi_read_num(SpiName_t spi_num) {
    uint8_t array[2];
#ifndef ESP32
    uint32_t init_int_cnt = SpiInstance[spi_num].it_cnt;
#endif
    memset(array,0xFF,sizeof(array));
    EXPECT_TRUE( spi_read(spi_num, array, sizeof(array)));
#ifndef ESP32
    EXPECT_GR(init_int_cnt, SpiInstance[spi_num].it_cnt);
#endif
    return true;
}

#ifdef HAS_SPI0
bool test_spi0_read(void) {
    gpio_set_state(DIO_SX1262_SS, 1);
#ifdef HAS_CAN
    gpio_set_state(DIO_CAN_SS, 1);
#endif
    EXPECT_TRUE(test_spi_read_num((SpiName_t)0) );
    return true;
}
#endif

#ifdef HAS_SPI1
bool test_spi1_read(void) {
    EXPECT_TRUE(test_spi_read_num((SpiName_t)1) );
    return true;
}
#endif

#ifdef HAS_SPI2
bool test_spi2_read(void) {
    EXPECT_TRUE(test_spi_read_num((SpiName_t)2) );
    return true;
}
#endif

#ifdef HAS_SPI3
bool test_spi3_read(void) {
    EXPECT_TRUE(test_spi_read_num((SpiName_t)3) );
    return true;
}
#endif
