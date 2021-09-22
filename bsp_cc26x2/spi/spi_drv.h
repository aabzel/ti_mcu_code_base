#ifndef SPI_DRV_H
#define SPI_DRV_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#ifdef DeviceFamily_CC26X2
#include <ti/drivers/SPI.h>
#endif /*DeviceFamily_CC26X2*/

#include "spi_common.h"

#define SPI0_BIT_RATE_HZ 2000000
#define SPI1_BIT_RATE_HZ 100000

/* SPI Board */
#define BOARD_SPI0_CSN PIN_UNASSIGNED

#ifdef DeviceFamily_CC26X2
extern SPI_Handle spi0Handle;
extern SPI_Params SPI0_Params;
#endif /*DeviceFamily_CC26X2*/

typedef enum eSpiName_t {
    SPI0_INX = 0,
    SPI1_INX = 1,

    SPI_CNT = 2
} SpiName_t;

extern SpiInstance_t SpiInstance[SPI_CNT];

bool spi_init(void);
bool spi_write(SpiName_t spi_num, const uint8_t* const tx_array, uint16_t tx_array_len);
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
