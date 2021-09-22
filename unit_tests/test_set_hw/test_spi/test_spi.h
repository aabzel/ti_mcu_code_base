#ifndef TEST_SPI_H
#define TEST_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_spi_write(void) ;
bool test_spi_read(void) ;


#define TEST_SUIT_SPI              \
    {"spi_write", test_spi_write}, \
    {"spi_read", test_spi_read},

#ifdef __cplusplus
}
#endif

#endif /* TEST_SPI_H */
