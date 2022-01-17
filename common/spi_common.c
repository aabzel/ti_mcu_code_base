#include "spi_common.h"

#include "sys_config.h"

#ifdef HAS_IO_BANG
#error "SPI component prohibited in IO-BANG build"
#endif

#ifdef SPI_COUNT
SpiInstance_t SpiInstance[SPI_COUNT];
#endif
