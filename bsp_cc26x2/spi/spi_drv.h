#ifndef SPI_DRV_H
#define SPI_DRV_H

#include <stdbool.h>
#include <stdint.h>

#ifdef DeviceFamily_CC26X2
#include <ti/drivers/SPI.h>
#endif /*DeviceFamily_CC26X2*/

/*!
 *  @def    BOARD_SPIName
 *  @brief  Enum of SPI names
 */
typedef enum eSpiName_t {
    SPI0_INX = 0,
    SPI1_INX = 1,

    SPI_CNT = 2
} SpiName_t;

#define SPI0_BIT_RATE_HZ 1000000
#define SPI1_BIT_RATE_HZ 100000

/* SPI Board */
#define BOARD_SPI0_MISO IOID_8 /* RF1.20 */
#define BOARD_SPI0_MOSI IOID_9 /* RF1.18 */
#define BOARD_SPI0_CLK IOID_10 /* RF1.16 */
#define BOARD_SPI0_CSN PIN_UNASSIGNED

#define BOARD_SPI1_MISO PIN_UNASSIGNED
#define BOARD_SPI1_MOSI PIN_UNASSIGNED
#define BOARD_SPI1_CLK PIN_UNASSIGNED
#define BOARD_SPI1_CSN PIN_UNASSIGNED

#ifdef DeviceFamily_CC26X2
extern SPI_Handle spi0Handle;
extern SPI_Params SPI0_Params;
#endif /*DeviceFamily_CC26X2*/

bool spi_init(void);
bool spi_write(uint8_t spi_num, uint8_t* array, uint16_t array_len);
bool spi_read(uint8_t spi_num, uint8_t* rx_array, uint16_t array_len);
#endif /* SPI_DRV_H  */
