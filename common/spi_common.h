#ifndef SPI_COMMON_H
#define SPI_COMMON_H

#include <stdbool.h>
#include <stdint.h>

#ifdef DeviceFamily_CC26X2
#include <ti/drivers/SPI.h>
#endif /*DeviceFamily_CC26X2*/

#ifdef USE_HAL_DRIVER
#include "stm32f4xx_hal.h"
#endif /*USE_HAL_DRIVER*/

#define SPI_NAME_SZ_BYTE 30
typedef struct xSpiInstance_t {
  bool tx_int;
  bool rx_int;
  volatile bool it_done;
  uint8_t *rx_buff;
  uint32_t rx_byte_cnt;
  uint32_t tx_byte_cnt;
  uint32_t rxtx_cnt;
  uint32_t tx_cnt;
  uint32_t rx_cnt;
  uint32_t err_cnt;
  volatile uint32_t it_cnt;
  uint32_t rx_buff_size;
  uint32_t tx_cpl_cnt;
#ifdef DeviceFamily_CC26X2
  uint32_t base_addr;
  SPI_Handle SpiHandle;
  SPI_Params SpiParams;
#endif
#ifdef USE_HAL_DRIVER
  SPI_HandleTypeDef handle;
#endif /*USE_HAL_DRIVER*/
  char name[SPI_NAME_SZ_BYTE];
  bool init_done;
}SpiInstance_t;

#endif /* SPI_COMMON_H */
