#ifndef SPI_DRV_H
#define SPI_DRV_H

#include <stdbool.h>
#include <stdint.h>

#ifdef DeviceFamily_CC26X2
#include <ti/drivers/SPI.h>
#endif /*DeviceFamily_CC26X2*/

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

typedef enum eSpiName_t {
    SPI0_INX = 0,
    SPI1_INX = 1,

    SPI_CNT = 2
} SpiName_t;

#define SPI_NAME_SZ_BYTE 40
typedef struct xSpiInstance_t {
    SPI_Handle SpiHandle;
    SPI_Params SpiParams;
    uint32_t rx_byte_cnt;
    uint32_t tx_byte_cnt;
    uint32_t base_addr;
    char name[SPI_NAME_SZ_BYTE];
    bool enable;
} SpiInstance_t;

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

#endif /* SPI_DRV_H  */
