#include "uart_drv.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "driver/uart.h"

#include "esp_log.h"
#include "bit_utils.h"
#include "uart_common.h"
#ifdef HAS_CLI
#include "uart_string_reader.h"
#endif
#include "sys_config.h"

uint32_t g_uart_rx_cnt = 0;
static volatile uint8_t rx_buff[UART_COUNT][1];

    uart_config_t uart_config = {
        .baud_rate = ECHO_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };



static bool init_uart(uint8_t uart_num, char* name) {
    bool res = false;
  
    return res;
}

bool uart_init(void) {
    bool res = true;
    int intr_alloc_flags = 0;

#if CONFIG_UART_ISR_IN_IRAM
    intr_alloc_flags = ESP_INTR_FLAG_IRAM;
#endif

    ESP_ERROR_CHECK(uart_driver_install(ECHO_UART_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(ECHO_UART_PORT_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(ECHO_UART_PORT_NUM, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS));


    return res;
}

static bool uart_send_ll(uint8_t uart_num, uint8_t* tx_buffer, uint16_t len) {
    bool res = false;
    uart_write_bytes(uart_num, (const char *) tx_buffer, len);
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



bool proc_uarts(void) { return false; }

uint32_t uart_get_baud_rate(uint8_t uart_num, uint16_t* mantissa, uint16_t* fraction, uint8_t* over_sampling) {
    bool res = true;
   
    return 0;
}
