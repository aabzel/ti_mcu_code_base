#include "spi_common.h"

#ifdef HAS_IO_BANG
#error "SPI component prohibited in IO BANG build"
#endif

#ifdef SPI_COUNT
SpiInstance_t SpiInstance[SPI_COUNT]={0};
#endif
