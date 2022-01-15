#ifndef SPI_COMMON_TYPE_H
#define SPI_COMMON_TYPE_H

#include <stdbool.h>
#include <stdint.h>

#ifdef ESP32
#include <spi_types.h>
#include <spi_master.h>
#endif

#include "sys_config.h"

#ifdef DeviceFamily_CC26X2
#include <ti/drivers/SPI.h>
#endif /*DeviceFamily_CC26X2*/

#ifdef USE_HAL_DRIVER
#include "stm32f4xx_hal.h"
#endif /*USE_HAL_DRIVER*/

#define SPI_NAME_SIZE_BYTE 30
typedef struct xSpiInstance_t {
  bool tx_int;
  bool rx_int;
  bool rxtx_int;
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
  uint32_t base_addr;
#ifdef DeviceFamily_CC26X2
  SPI_Handle SpiHandle;
  SPI_Params SpiParams;
#endif
#ifdef USE_HAL_DRIVER
  SPI_HandleTypeDef handle;
#endif /*USE_HAL_DRIVER*/
#ifdef ESP32
  spi_device_handle_t spi_device_handle;
  spi_host_device_t host_id;
#endif /*ESP32*/
  char name[SPI_NAME_SIZE_BYTE];
  bool init_done;
}SpiInstance_t;

#ifdef SPI_COUNT
extern SpiInstance_t SpiInst[SPI_COUNT];
#endif

#endif /* SPI_COMMON_TYPE_H */
