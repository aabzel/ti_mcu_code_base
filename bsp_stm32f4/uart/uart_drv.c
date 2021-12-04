#include "uart_drv.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "stm32f4xx_hal.h"
#include "uart_common.h"
#ifdef HAS_CLI
#include "uart_string_reader.h"
#endif
#include "sys_config.h"

uint32_t g_uart_rx_cnt = 0;
static  volatile uint8_t rx_buff[UART_COUNT][1];

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
static bool init_uart(uint8_t uart_num) {
    bool res = false;
    huart[uart_num].rx_buff = (volatile uint8_t *) rx_buff[uart_num][0];
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
    HAL_UART_Receive_IT(&huart[uart_num].uart_h, (uint8_t*)huart[uart_num].rx_buff, huart[uart_num].rx_buff_size);
	char str[20]="0";
    snprintf(str,sizeof(str),"UART%u",uart_num);
    uart_send(uart_num, (uint8_t *)str, strlen(str), true);
    return res;
}

bool uart_init(void) {
    bool res = true;
    res = init_uart(UART_NUM_CLI) && res;

    return res;
}

int cli_putchar_uart(int ch) { return ch; }

void cli_tune_read_char(void) {}

bool uart_send_ll(uint8_t uart_num, uint8_t* tx_buffer, uint16_t len) {
    bool res = false;
    HAL_StatusTypeDef stat;
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
    if(uart_num < UART_COUNT) {
        res = uart_send_ll(uart_num, array, array_len);
    }
    return res;
}

bool uart_read(uint8_t uart_num, uint8_t* out_array, uint16_t array_len) {
    bool res = false;

    return res;
}

uint32_t uart_get_baud_rate(uint8_t uart_num, uint16_t* mantissa, uint16_t* fraction, uint8_t* over_sampling) {
    return 0;
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
        huart[UART_NUM_CLI-1].rx_cnt++;
        huart[UART_NUM_CLI-1].rx_int = true;
        HAL_UART_Receive_IT(&huart[UART_NUM_CLI-1].uart_h, (uint8_t*)huart[UART_NUM_CLI-1].rx_buff,
                            huart[UART_NUM_CLI-1].rx_buff_size);
    }
#endif
    if(USART6 == uart_handle->Instance) {
        huart[UART_NUM_CLI-1].rx_cnt++;
        huart[UART_NUM_CLI-1].rx_int = true;
        HAL_UART_Receive_IT(&huart[UART_NUM_CLI-1].uart_h, (uint8_t*)huart[UART_NUM_CLI-1].rx_buff,
                            huart[UART_NUM_CLI-1].rx_buff_size);
#ifdef HAS_CLI
        uart_string_reader_rx_callback(&cmd_reader, huart[UART_NUM_CLI-1].rx_buff[0]);
#endif /*HAS_CLI*/
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* uart_handle) {
#ifdef UART7
    if(UART7 == uart_handle->Instance) {
        huart[UART_NUM_CLI-1].tx_cnt++;
#ifdef HAS_CLI
        huart[UART_NUM_CLI-1].tx_int = true;
        huart[UART_NUM_CLI-1].tx_cpl_cnt++;
#endif /*HAS_CLI*/
    }
#endif
    if(USART6 == uart_handle->Instance) {
        huart[UART_NUM_CLI-1].tx_cnt++;
#ifdef HAS_CLI
        huart[UART_NUM_CLI-1].tx_int = true;
        huart[UART_NUM_CLI-1].tx_cpl_cnt++;
#endif /*HAS_CLI*/
    }
}

bool proc_uarts(void) {
	return false;
}