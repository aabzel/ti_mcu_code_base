#ifndef SPI_DRV_H
#define SPI_DRV_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "spi_common.h"
#include "stm32f4xx_hal.h"
  
#define SPI1_BIT_RATE_HZ 100000

typedef enum eSpiName_t {
    SPI_UNDEF_INX = 0,
    SPI1_INX = 1,
    SPI2_INX = 2,
    SPI3_INX = 3,
    SPI4_INX = 4,
    SPI5_INX = 5,
    SPI_CNT = 6
} SpiName_t;

extern SpiInstance_t SpiInstance[SPI_CNT];

bool spi_init(void);
bool spi_write(SpiName_t spi_num, uint8_t* array, uint16_t array_len);
bool spi_read(SpiName_t spi_num, uint8_t* rx_array, uint16_t array_len);
uint32_t spi_get_clock(SpiName_t spi_num);
uint8_t spi_get_phase(SpiName_t spi_num);
uint8_t spi_get_polarity(SpiName_t spi_num);
uint8_t spi_get_data_size(SpiName_t spi_num);
uint8_t spi_get_transmit_int(SpiName_t spi_num);
uint8_t spi_get_receive_int(SpiName_t spi_num);
uint8_t spi_get_receive_timeout_interrupt(SpiName_t spi_num);
uint8_t spi_get_receive_overrun_interrupt(SpiName_t spi_num);

#ifdef __cplusplus
}
#endif

#endif /* SPI_DRV_H  */
