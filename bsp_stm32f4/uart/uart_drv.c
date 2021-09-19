#include "uart_drv.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "uart_common.h"
#include "stm32f4xx_hal.h"
#ifdef HAS_CLI
#include "uart_string_reader.h"
#endif

uint32_t g_uart_rx_cnt=0;
UartHandle_t huart[UART_COUNT] = {0};
static uint8_t rx_buff7[1];

static bool init_uart7(void){
  bool res = false;
  huart[CLI_UART_NUM].rx_buff=rx_buff7;
  huart[CLI_UART_NUM].rx_buff_size = sizeof(rx_buff7);
  huart[CLI_UART_NUM].uart_h.Instance = UART7;
  huart[CLI_UART_NUM].uart_h.Init.BaudRate = CLI_UART_BAUD_RATE;
  huart[CLI_UART_NUM].uart_h.Init.WordLength = UART_WORDLENGTH_8B;
  huart[CLI_UART_NUM].uart_h.Init.StopBits = UART_STOPBITS_1;
  huart[CLI_UART_NUM].uart_h.Init.Parity = UART_PARITY_NONE;
  huart[CLI_UART_NUM].uart_h.Init.Mode = UART_MODE_TX_RX;
  huart[CLI_UART_NUM].uart_h.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart[CLI_UART_NUM].uart_h.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart[CLI_UART_NUM].uart_h) != HAL_OK)  {
    res = false;
  }else{
     res = true;
     huart[CLI_UART_NUM].init_done=true;
  }
  HAL_UART_Receive_IT(&huart[CLI_UART_NUM].uart_h, huart[CLI_UART_NUM].rx_buff, huart[CLI_UART_NUM].rx_buff_size);
  uart_send(7, "uart7", 5);
  return res;
}

bool uart_init(void) {
    bool res = true;
    res = init_uart7() && res;
    return res;
}

int cli_putchar_uart(int ch) {
    return ch;
}

void cli_tune_read_char(void) { 

}

bool uart_send_ll(uint8_t uart_num, uint8_t* tx_buffer, uint16_t len) {
    bool res = false;
    HAL_StatusTypeDef stat;
    uint32_t cnt=0;
    uint32_t init_tx_cnt = huart[uart_num].tx_cnt;
    stat = HAL_UART_Transmit_IT(&huart[uart_num].uart_h, tx_buffer, len);
    if (HAL_OK == stat) {
      res = true;
      while (init_tx_cnt == huart[uart_num].tx_cnt) {
        cnt++;
        if(0x00FFFFFF<cnt){
          res = false;
          break;
        }
      }
    }
    return res;
}

bool uart_send(uint8_t uart_num, uint8_t* array, uint16_t array_len) {
    bool res = false;
    if(uart_num < UART_COUNT) {
        res = uart_send_ll(uart_num, array, array_len);
    }
    return res;
}

bool uart_read(uint8_t uart_num, uint8_t* out_array, uint16_t array_len){
    bool res = false; 

    return res;
}

uint32_t uart_get_baud_rate(uint8_t uart_num, uint16_t* mantissa, uint16_t* fraction, uint8_t* over_sampling) {
    return 0;
}

bool uart_set_baudrate(uint8_t uart_num, uint32_t baudrate) { return false; }

void HAL_UART_MspInit(UART_HandleTypeDef* uart_handle){
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uart_handle->Instance==UART7)  {
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
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uart_handle){
  if(uart_handle->Instance==UART7)  {
    __HAL_RCC_UART7_CLK_DISABLE();

    /**UART7 GPIO Configuration
    PE8     ------> UART7_TX
    PE7     ------> UART7_RX
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_8|GPIO_PIN_7);

    HAL_NVIC_DisableIRQ(UART7_IRQn);
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *uart_handle) {
  g_uart_rx_cnt++;
  if (UART7 == uart_handle->Instance) {
      huart[CLI_UART_NUM].rx_cnt++;
      huart[CLI_UART_NUM].rx_int = true;
      HAL_UART_Receive_IT(&huart[CLI_UART_NUM].uart_h, huart[CLI_UART_NUM].rx_buff, huart[CLI_UART_NUM].rx_buff_size);
#ifdef HAS_CLI
      uart_string_reader_rx_callback(&cmd_reader, huart[CLI_UART_NUM].rx_buff[0]);
#endif /*HAS_CLI*/
  }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* uart_handle) {
    if (UART7 == uart_handle->Instance) {
        huart[CLI_UART_NUM].tx_cnt++;
#ifdef HAS_CLI
        huart[CLI_UART_NUM].tx_int = true;
        huart[CLI_UART_NUM].tx_cpl_cnt++;
#endif /*HAS_CLI*/
    }
}

