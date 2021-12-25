#include "spi_drv.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "bit_utils.h"
#include "clocks.h"
#include "sys_config.h"
#include "gpio_drv.h"
#include "stm32f4xx_hal.h"

static SpiName_t spi_base_2_num(SPI_TypeDef* Instance) {
    SpiName_t spi_num = (SpiName_t)0;
    if(SPI1 == Instance) {
        spi_num = (SpiName_t)1;
    } else if(SPI2 == Instance) {
        spi_num = (SpiName_t)2;
    } else if(SPI3 == Instance) {
        spi_num = (SpiName_t)3;
#ifdef HAS_SPI4
    } else if(SPI4 == Instance) {
        spi_num = (SpiName_t)4;
#endif
#ifdef HAS_SPI5
    } else if(SPI5 == Instance) {
        spi_num = (SpiName_t)5;
#endif
    } else {
        spi_num = (SpiName_t)0;
    }
    return spi_num;
}

static SPI_TypeDef* spi_num_2_base(SpiName_t spi_num) {
    SPI_TypeDef* Instance = 0;
    switch(spi_num) {
#ifdef HAS_SPI1
    case 1:
        Instance = SPI1;
        break;
#endif
    case 2:
        Instance = SPI2;
        break;
    case 3:
        Instance = SPI3;
        break;
#ifdef HAS_SPI4
    case 4:
        Instance = SPI4;
        break;
#endif
#ifdef HAS_SPI5
    case 5:
        Instance = SPI5;
        break;
#endif
    default:
        Instance = NULL;
        break;
    }
    return Instance;
}

static uint32_t BitRate2Prescaler(uint32_t bit_rate, uint32_t bus_freq){
    uint32_t prescaler = 0;
    uint32_t i = 0;
    for(i=1;i<=8;i++){

    }
    return prescaler;
}

static bool spi_init_ll(SpiName_t spi_num, char* spi_name, uint32_t bit_rate) {
    bool res = false;
    uint32_t prescaler =BitRate2Prescaler(bit_rate,APB1_CLOCK_HZ);
    SpiInstance[spi_num - 1].handle.Init.Mode = SPI_MODE_MASTER;
    SpiInstance[spi_num - 1].handle.Instance = spi_num_2_base(spi_num);
    SpiInstance[spi_num - 1].handle.Init.Direction = SPI_DIRECTION_2LINES;
    SpiInstance[spi_num - 1].handle.Init.DataSize = SPI_DATASIZE_8BIT;
    SpiInstance[spi_num - 1].handle.Init.CLKPolarity = SPI_POLARITY_LOW;
    SpiInstance[spi_num - 1].handle.Init.CLKPhase = SPI_PHASE_1EDGE;
    SpiInstance[spi_num - 1].handle.Init.NSS = SPI_NSS_SOFT;
    SpiInstance[spi_num - 1].handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    SpiInstance[spi_num - 1].handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
    SpiInstance[spi_num - 1].handle.Init.TIMode = SPI_TIMODE_DISABLE;
    SpiInstance[spi_num - 1].handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    SpiInstance[spi_num - 1].handle.Init.CRCPolynomial = 10;
    strncpy(SpiInstance[spi_num - 1].name, spi_name, sizeof(SpiInstance[spi_num - 1].name));
    if(HAL_OK == HAL_SPI_Init(&SpiInstance[spi_num - 1].handle)) {
        res = true;
        SpiInstance[spi_num - 1].init_done = true;
    } else {
        res = false;
    }
    return res;
}

bool spi_init(void) {
    bool res = true;
#ifdef HAS_SPI1
    res = spi_init_ll((SpiName_t)1, "NorFlash", SPI1_BIT_RATE_HZ) && res;
#endif /*HAS_SPI1*/
#ifdef HAS_SPI2
    res = spi_init_ll((SpiName_t)2, "SX1262", SPI2_BIT_RATE_HZ) && res;
#endif /*HAS_SPI1*/
    return res;
}

bool spi_write(SpiName_t spi_num, uint8_t* tx_array, uint16_t tx_array_len) {
    bool res = false;
    HAL_StatusTypeDef status = HAL_ERROR;
    (void)status;
    switch(spi_num) {
    case 0:
        res = true;
        break;
#ifdef HAS_SPI1
    case 1:
        res = true;
        break;
#endif
#ifdef HAS_SPI2
    case 2:
        res = true;
        break;
#endif
    default:
        res = false;
        break;
    }
    if(res) {
        status = HAL_SPI_Transmit_IT(&SpiInstance[spi_num - 1].handle, tx_array, tx_array_len);
        if(HAL_OK == status) {
            res = true;
        }
        SpiInstance[spi_num - 1].tx_byte_cnt += tx_array_len;
    }
    return res;
}

bool spi_read(SpiName_t spi_num, uint8_t* rx_array, uint16_t rx_array_len) {
    bool res = false;
    HAL_StatusTypeDef status = HAL_ERROR;
    (void)status;
    switch(spi_num) {
    case 0:
        break;
#ifdef HAS_SPI1
    case 1:
        break;
#endif
#ifdef HAS_SPI2
    case 2:
        res = true;
        break;
#endif
    default:
        res = false;
        break;
    }
    if(res) {
        status = HAL_SPI_Receive_IT(&SpiInstance[spi_num - 1].handle, rx_array, rx_array_len);
        if(HAL_OK == status) {
            res = true;
        }
        SpiInstance[spi_num - 1].rx_byte_cnt += rx_array_len;
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

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    (void)GPIO_InitStruct;
#ifdef HAS_SPI1
    if(spiHandle->Instance == SPI1) {
        __HAL_RCC_SPI1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**SPI1 GPIO Configuration
        PA5     ------> SPI1_SCK
        PA6     ------> SPI1_MISO
        PA7     ------> SPI1_MOSI
        */
        GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(SPI1_IRQn, 7, 0);
        HAL_NVIC_EnableIRQ(SPI1_IRQn);
    }
#endif

#ifdef HAS_SPI2
    if(SPI2==spiHandle->Instance) {
        __HAL_RCC_SPI2_CLK_ENABLE();
        HAL_NVIC_SetPriority(SPI2_IRQn, 7, 0);
        HAL_NVIC_EnableIRQ(SPI2_IRQn);
    }
#endif
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle) {
#ifdef HAS_SPI1
    if(spiHandle->Instance == SPI1) {
        __HAL_RCC_SPI1_CLK_DISABLE();

        /**SPI1 GPIO Configuration
        PA5     ------> SPI1_SCK
        PA6     ------> SPI1_MISO
        PA7     ------> SPI1_MOSI
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

        HAL_NVIC_DisableIRQ(SPI1_IRQn);
    }
#endif
#ifdef HAS_SPI2
    if(spiHandle->Instance == SPI2) {
        __HAL_RCC_SPI2_CLK_DISABLE();

        HAL_NVIC_DisableIRQ(SPI2_IRQn);
    }
#endif
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi) {
    bool res = false;
    SpiName_t spi_num = spi_base_2_num(hspi->Instance);
#ifdef HAS_SPI1
    if(SPI1 == hspi->Instance) {
        SpiInstance[spi_num - 1].tx_cnt++;
        res = true;
    }
#endif
#ifdef HAS_SPI2
    if(SPI2 == hspi->Instance) {
        res = true;
    }
#endif
    if(res) {
        SpiInstance[spi_num - 1].tx_cnt++;
    }
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef* hspi) {
    bool res = false;
    SpiName_t spi_num = spi_base_2_num(hspi->Instance);
#ifdef HAS_SPI1
    if(SPI1 == hspi->Instance) {
        res = true;
    }
#endif
#ifdef HAS_SPI2
    if(SPI2 == hspi->Instance) {
        res = true;
    }
#endif
    if(res) {
        SpiInstance[spi_num - 1].rxtx_cnt++;
    }
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef* hspi) {
    SpiName_t spi_num = spi_base_2_num(hspi->Instance);
    bool res = false;
#ifdef HAS_SPI1
    if(SPI1 == hspi->Instance) {
        res = true;
    }
#endif
#ifdef HAS_SPI2
    if(SPI2 == hspi->Instance) {
        res = true;
    }
#endif
    if(res) {
        SpiInstance[spi_num - 1].rx_cnt++;
    }
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef* hspi) {
    SpiName_t spi_num = spi_base_2_num(hspi->Instance);
    bool res = false;
#ifdef HAS_SPI1
    if(SPI1 == hspi->Instance) {
        res = true;
    }
#endif
#ifdef HAS_SPI2
    if(SPI2 == hspi->Instance) {
        res = true;
    }
#endif
    if(res) {
        SpiInstance[spi_num - 1].err_cnt++;
        hspi->State = HAL_SPI_STATE_READY;
        hspi->ErrorCode = 0;
        hspi->Instance->SR = 0x0002;
    }
}
