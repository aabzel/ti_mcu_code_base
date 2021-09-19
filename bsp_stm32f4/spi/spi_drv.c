#include "spi_drv.h"


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "bit_utils.h"
#include "clocks.h"
#include "gpio_drv.h"
#include "stm32f4xx_hal.h"


SpiInstance_t SpiInstance[SPI_CNT];


static SpiName_t spi_base_2_num(SPI_TypeDef *Instance){
    SpiName_t spi_num=(SpiName_t)0;
    if(SPI1==Instance){
      spi_num=(SpiName_t)1;
    }else if(SPI2==Instance){
      spi_num=(SpiName_t)2;
    }else if(SPI3==Instance){
      spi_num=(SpiName_t)3;
    }else if(SPI4==Instance){
      spi_num=(SpiName_t)4;
    }else if(SPI5==Instance){
      spi_num=(SpiName_t)5;
    }else {
      spi_num=(SpiName_t)0;
    }
    return spi_num;
}

static SPI_TypeDef*  spi_num_2_base(SpiName_t spi_num){
    SPI_TypeDef* Instance=0;
    switch(spi_num){
      case 1: Instance=SPI1; break;
      case 2: Instance=SPI2; break;
      case 3: Instance=SPI3; break;
      case 4: Instance=SPI4; break;
      case 5: Instance=SPI5; break;
    default:
      Instance=NULL;
      break;
    }
    return Instance;
}

static bool spi_init_ll(SpiName_t spi_num, char* spi_name, uint32_t bit_rate) {
  bool res = false;
  SpiInstance[spi_num].handle.Init.Mode = SPI_MODE_MASTER;
  SpiInstance[spi_num].handle.Instance =  spi_num_2_base(spi_num);
  SpiInstance[spi_num].handle.Init.Direction = SPI_DIRECTION_2LINES;
  SpiInstance[spi_num].handle.Init.DataSize = SPI_DATASIZE_8BIT;
  SpiInstance[spi_num].handle.Init.CLKPolarity = SPI_POLARITY_LOW;
  SpiInstance[spi_num].handle.Init.CLKPhase = SPI_PHASE_1EDGE;
  SpiInstance[spi_num].handle.Init.NSS = SPI_NSS_SOFT;
  SpiInstance[spi_num].handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  SpiInstance[spi_num].handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
  SpiInstance[spi_num].handle.Init.TIMode = SPI_TIMODE_DISABLE;
  SpiInstance[spi_num].handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  SpiInstance[spi_num].handle.Init.CRCPolynomial = 10;
  strncpy(SpiInstance[spi_num].name,spi_name,sizeof(SpiInstance[spi_num].name));
  if (HAL_OK == HAL_SPI_Init(&SpiInstance[spi_num].handle) )  {
     res = true;
     SpiInstance[spi_num].init_done = true;
  }else{
     res = false;
  }
  return res;
}




bool spi_init(void) {
    bool res = true;
#ifdef HAS_SPI1
    res = spi_init_ll((SpiName_t)1, "NorFlash", SPI1_BIT_RATE_HZ) && res;
#endif /*HAS_SPI1*/
    return res;
}

bool spi_write(SpiName_t spi_num, uint8_t* tx_array, uint16_t tx_array_len) {
    bool res = false;
    HAL_StatusTypeDef status;
    switch(spi_num) {
    case 0:
        break;
    case 1:
        status = HAL_SPI_Transmit_IT(&SpiInstance[spi_num].handle,tx_array,tx_array_len);
        if (HAL_OK==status ) {
          res = true;
        }
        break;
    default:
        res = false;
        break;
    }
    if(res) {
        SpiInstance[spi_num].tx_byte_cnt += tx_array_len;
    }
    return res;
}

bool spi_read(SpiName_t spi_num, uint8_t* rx_array, uint16_t rx_array_len) {
    bool res = false;
    HAL_StatusTypeDef status;
    switch(spi_num) {
    case 0:
        break;
    case 1:
        status = HAL_SPI_Receive_IT(&SpiInstance[spi_num].handle,rx_array,rx_array_len);
        if (HAL_OK==status ) {
          res = true;
        }
        break;
    default:
        res = false;
        break;
    }
    if(res) {
        SpiInstance[spi_num].rx_byte_cnt += rx_array_len;
    }

    return res;
}

uint32_t spi_get_clock(SpiName_t spi_num) {
    uint32_t spi_bit_rate = 0xFF;
    return spi_bit_rate;
}

uint8_t spi_get_phase(SpiName_t spi_num) {
    uint32_t phase = 9;
    return phase;
}

uint8_t spi_get_polarity(SpiName_t spi_num) {
    uint32_t polarity = 9;
    return polarity;
}

uint8_t spi_get_data_size(SpiName_t spi_num) {
    uint32_t data_size = 0xFF;
    return data_size;
}

uint8_t spi_get_transmit_int(SpiName_t spi_num) {
    uint8_t val = 0xFF;
    return val;
}

uint8_t spi_get_receive_int(SpiName_t spi_num) {
    uint8_t val = 0xFF;
    return val;
}

uint8_t spi_get_receive_timeout_interrupt(SpiName_t spi_num) {
    uint8_t val = 0xFF;
    return val;
}

uint8_t spi_get_receive_overrun_interrupt(SpiName_t spi_num) {
    uint8_t val = 0xFF;
    return val;
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle){

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI1)  {
    __HAL_RCC_SPI1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(SPI1_IRQn, 7, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle){

  if(spiHandle->Instance==SPI1)  {
    __HAL_RCC_SPI1_CLK_DISABLE();

    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

    HAL_NVIC_DisableIRQ(SPI1_IRQn);
  }
}



void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
  SpiName_t spi_num = spi_base_2_num(hspi->Instance);
  if (hspi->Instance == SPI1) {
    SpiInstance[spi_num].tx_cnt++;
  }
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
  SpiName_t spi_num = spi_base_2_num(hspi->Instance);
  if (hspi->Instance == SPI1) {
    SpiInstance[spi_num].rxtx_cnt++;
  }
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) {
  SpiName_t spi_num = spi_base_2_num(hspi->Instance);
  if (SPI1 == hspi->Instance) {
    SpiInstance[spi_num].rx_cnt++;
  }
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi) {
  SpiName_t spi_num = spi_base_2_num(hspi->Instance);
  if (SPI1 == hspi->Instance) {
    SpiInstance[spi_num].err_cnt++;
    hspi->State = HAL_SPI_STATE_READY;
    hspi->ErrorCode = 0;
    hspi->Instance->SR = 0x0002;
  }
}

