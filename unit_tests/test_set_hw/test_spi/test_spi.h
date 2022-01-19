#ifndef TEST_SPI_H
#define TEST_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#ifdef HAS_SPI_WAIT_WRITE
bool test_spi_wait_write(void) ;
#define TEST_SUIT_SPI_WAIT_WRITE              \
    {"spi_wait_write", test_spi_wait_write},

#else
#define TEST_SUIT_SPI_WAIT_WRITE
#endif

bool test_spi_write(void) ;
bool test_spi_read(void) ;
#define TEST_SUIT_SPI_GENERAL              \
    {"spi_write", test_spi_write}, \
    {"spi_read", test_spi_read},


#define TEST_SUIT_SPI \
    TEST_SUIT_SPI_GENERAL \
    TEST_SUIT_SPI_WAIT_WRITE

#ifdef __cplusplus
}
#endif

#endif /* TEST_SPI_H */
