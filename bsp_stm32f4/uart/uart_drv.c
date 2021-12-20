#include "uart_drv.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "bit_utils.h"
#include "stm32f4xx_hal.h"
#include "uart_common.h"
#ifdef HAS_CLI
#include "uart_string_reader.h"
#endif
#include "sys_config.h"

uint32_t g_uart_rx_cnt = 0;
static volatile uint8_t rx_buff[UART_COUNT][1];

static USART_TypeDef* uart_get_instance(uint8_t uart_num) {
    USART_TypeDef* USARTx = NULL;
    switch(uart_num) {
    case 5:
        USARTx = USART6;
        break;
    default:
        break;
    }
    return USARTx;
}
#if 0
static uint8_t get_uart_index(USART_TypeDef* USARTx) {
    uint8_t uart_num = 0;
    if(USART1 == USARTx) {
        uart_num = 1;
    } else if(USART2 == USARTx) {
        uart_num = 2;
    } else if(USART3 == USARTx) {
        uart_num = 3;
    } else if(UART4 == USARTx) {
        uart_num = 4;
    } else if(UART5 == USARTx) {
        uart_num = 5;
    } else if(USART6 == USARTx) {
        uart_num = 6;
#ifdef UART7
    } else if(UART7 == USARTx) {
        uart_num = 7;
#endif /*UART7*/
#ifdef UART8
    } else if(UART8 == USARTx) {
        uart_num = 8;
#endif /*UART8*/
#ifdef UART9
    } else if(UART9 == USARTx) {
        uart_num = 9;
#endif /*UART9*/
#ifdef UART10
    } else if(UART10 == USARTx) {
        uart_num = 10;
#endif /*UART10*/
    } else {
        uart_num = 0;
    }

    return uart_num;
}
#endif

static bool init_uart(uint8_t uart_num, char* name) {
    bool res = false;
    strncpy(huart[uart_num].name, name, sizeof(huart[uart_num].name));
    huart[uart_num].rx_buff = (volatile uint8_t*)&rx_buff[uart_num][0];
    huart[uart_num].rx_buff_size = sizeof(rx_buff[uart_num][0]);
    huart[uart_num].uart_h.Init.BaudRate = CLI_UART_BAUD_RATE;
    huart[uart_num].uart_h.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart[uart_num].uart_h.Init.Mode = UART_MODE_TX_RX;
    huart[uart_num].uart_h.Init.OverSampling = UART_OVERSAMPLING_16;
    huart[uart_num].uart_h.Init.Parity = UART_PARITY_NONE;
    huart[uart_num].uart_h.Init.StopBits = UART_STOPBITS_1;
    huart[uart_num].uart_h.Init.WordLength = UART_WORDLENGTH_8B;
    huart[uart_num].uart_h.Instance = uart_get_instance(uart_num);

    if(HAL_UART_Init(&huart[uart_num].uart_h) != HAL_OK) {
        res = false;
    } else {
        res = true;
        huart[uart_num].init_done = true;
    }
    HAL_StatusTypeDef ret =
        HAL_UART_Receive_IT(&huart[uart_num].uart_h, (uint8_t*)huart[uart_num].rx_buff, huart[uart_num].rx_buff_size);
    if(HAL_OK != ret) {
        res = false;
    }
    char str[20] = "0";
    snprintf(str, sizeof(str), "UART%u", uart_num + 1);
    uart_send(uart_num, (uint8_t*)str, strlen(str), true);
    return res;
}

bool uart_init(void) {
    bool res = true;
    res = init_uart(UART_NUM_CLI, "CLI") && res;
    if(res) {
        res = uart_send_banner(UART_NUM_CLI, 'v');
    }

    return res;
}

static bool uart_send_ll(uint8_t uart_num, uint8_t* tx_buffer, uint16_t len) {
    bool res = false;
    HAL_StatusTypeDef stat = HAL_ERROR;
    uint32_t cnt = 0;
    uint32_t init_tx_cnt = huart[uart_num].tx_cnt;
    stat = HAL_UART_Transmit_IT(&huart[uart_num].uart_h, tx_buffer, len);
    if(HAL_OK == stat) {
        res = true;
        while(init_tx_cnt == huart[uart_num].tx_cnt) {
            cnt++;
            if(0x00FFFFFF < cnt) {
                res = false;
                break;
            }
        }
    }
    return res;
}

bool uart_send(uint8_t uart_num, uint8_t* array, uint16_t array_len, bool is_wait) {
    bool res = false;
    (void)is_wait;
    if(array && (uart_num < UART_COUNT) && (0 < array_len)) {
        res = uart_send_ll(uart_num, array, array_len);
    }
    return res;
}

bool uart_read(uint8_t uart_num, uint8_t* out_array, uint16_t array_len) {
    bool res = false;

    return res;
}

bool uart_set_baudrate(uint8_t uart_num, uint32_t baudrate) { return false; }

void HAL_UART_MspInit(UART_HandleTypeDef* uart_handle) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
#ifdef UART7
    if(uart_handle->Instance == UART7) {
        __HAL_RCC_UART7_CLK_ENABLE();

        __HAL_RCC_GPIOE_CLK_ENABLE();
        /**UART7 GPIO Configuration
        PE8     ------> UART7_TX
        PE7     ------> UART7_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF8_UART7;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
        HAL_NVIC_SetPriority(UART7_IRQn, 7, 0);
        HAL_NVIC_EnableIRQ(UART7_IRQn);
    }
#endif
    if(USART6 == uart_handle->Instance) {
        __HAL_RCC_USART6_CLK_ENABLE();

        __HAL_RCC_GPIOC_CLK_ENABLE();
        /**USART6 GPIO Configuration
        PC6     ------> USART6_TX
        PC7     ------> USART6_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(USART6_IRQn, 7, 0);
        HAL_NVIC_EnableIRQ(USART6_IRQn);
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uart_handle) {
#ifdef UART7
    if(uart_handle->Instance == UART7) {
        __HAL_RCC_UART7_CLK_DISABLE();
        /**UART7 GPIO Configuration
        PE8     ------> UART7_TX
        PE7     ------> UART7_RX
        */
        HAL_GPIO_DeInit(GPIOE, GPIO_PIN_8 | GPIO_PIN_7);

        HAL_NVIC_DisableIRQ(UART7_IRQn);
    }
#endif
    if(USART6 == uart_handle->Instance) {
        __HAL_RCC_USART6_CLK_DISABLE();
        /**USART6 GPIO Configuration
        PC6     ------> USART6_TX
        PC7     ------> USART6_RX
        */
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6 | GPIO_PIN_7);
        /* USART6 interrupt Deinit */
        HAL_NVIC_DisableIRQ(USART6_IRQn);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* uart_handle) {
    g_uart_rx_cnt++;
#ifdef UART7
    if(UART7 == uart_handle->Instance) {
        huart[6].rx_cnt++;
        huart[6].rx_int = true;
        HAL_UART_Receive_IT(&huart[6].uart_h, (uint8_t*)huart[6].rx_buff, huart[6].rx_buff_size);
    }
#endif
    if(USART6 == uart_handle->Instance) {
        huart[5].rx_cnt++;
        huart[5].rx_int = true;
        HAL_UART_Receive_IT(&huart[5].uart_h, (uint8_t*)huart[5].rx_buff, huart[5].rx_buff_size);
#ifdef HAS_CLI
        uart_string_reader_rx_callback(&cmd_reader, huart[5].rx_buff[0]);
#endif /*HAS_CLI*/
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* uart_handle) {
#ifdef UART7
    if(UART7 == uart_handle->Instance) {
        huart[6].tx_cnt++;
#ifdef HAS_CLI
        huart[6].tx_int = true;
        huart[6].tx_cpl_cnt++;
#endif /*HAS_CLI*/
    }
#endif
    if(USART6 == uart_handle->Instance) {
        huart[5].tx_cnt++;
#ifdef HAS_CLI
        huart[5].tx_int = true;
        huart[5].tx_cpl_cnt++;
#endif /*HAS_CLI*/
    }
}

bool proc_uarts(void) { return false; }

/*see Figure 4. STM32F413xG/H block diagram*/
static const uint32_t UartClockLUT[UART_COUNT] = {
    /*0  USART1  */ APB2_CLOCK_HZ,
    /*1  USART2  */ APB1_CLOCK_HZ,
    /*2  USART3  */ APB1_CLOCK_HZ,
    /*3  UART4  */ APB1_CLOCK_HZ,
    /*4  UART5  */ APB1_CLOCK_HZ,
    /*5  USART6  */ APB2_CLOCK_HZ,
};

uint32_t uart_get_baud_rate(uint8_t uart_num, uint16_t* mantissa, uint16_t* fraction, uint8_t* over_sampling) {
    bool res = true;
    uint32_t baud_rate_reg = 0;
    uint32_t control_register1 = 0;
    uint32_t baud_rate = 0;
    switch(uart_num) {
    case 0:
        baud_rate_reg = USART1->BRR;
        control_register1 = USART1->CR1;
        break;
    case 1:
        baud_rate_reg = USART2->BRR;
        control_register1 = USART2->CR1;
        break;
    case 2:
        baud_rate_reg = USART3->BRR;
        control_register1 = USART3->CR1;
        break;
    case 3:
        baud_rate_reg = UART4->BRR;
        control_register1 = UART4->CR1;
        break;
    case 4:
        baud_rate_reg = UART5->BRR;
        control_register1 = UART5->CR1;
        break;
    case 5:
        baud_rate_reg = USART6->BRR;
        control_register1 = USART6->CR1;
        break;
    case 6:
#ifdef UART7
        baud_rate_reg = UART7->BRR;
        control_register1 = UART7->CR1;
#endif
        break;
    case 7:
#ifdef UART8
        baud_rate_reg = UART8->BRR;
        control_register1 = UART8->CR1;
#endif
        break;
    case 8:
#ifdef UART9
        baud_rate_reg = UART9->BRR;
        control_register1 = UART9->CR1;
#endif
        break;
    case 9:
#ifdef UART10
        baud_rate_reg = UART10->BRR;
        control_register1 = UART10->CR1;
#endif
        break;
    default:
        res = false;
        break;
    }
    if(true == res) {
        (*fraction) = MASK_4BIT & baud_rate_reg;
        (*mantissa) = MASK_12BIT & (baud_rate_reg >> 4);
        if(BIT_15 == (control_register1 & BIT_15)) {
            (*over_sampling) = 8U;
        } else {
            (*over_sampling) = 16U;
        }
        baud_rate = UartClockLUT[uart_num] / ((*over_sampling) * ((*mantissa) + (*fraction) / (*over_sampling)));
    }

    return baud_rate;
}
