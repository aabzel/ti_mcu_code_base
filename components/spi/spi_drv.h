#ifndef SPI_DRV_H
#define SPI_DRV_H

#include <stdbool.h>
#include <stdint.h>

bool spi_init(void);
bool spi_send(uint8_t spi_num, uint8_t *array, uint16_t  array_len) ;

#endif /* SPI_DRV_H  */
