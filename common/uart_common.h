#ifndef UART_COMMON_H
#define UART_COMMON_H

#include <stdbool.h>
#include <stdint.h>

#include "fifo_char.h"

#ifdef DeviceFamily_CC26X2
#include <ti/devices/cc13x2_cc26x2/inc/hw_ints.h>
#include <ti/devices/cc13x2_cc26x2/inc/hw_memmap.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/power/PowerCC26X2.h>
#include <ti/drivers/uart/UARTCC26XX.h>
#endif

#ifdef USE_HAL_DRIVER
#include "stm32f4xx_hal.h"
#endif /*USE_HAL_DRIVER*/

#ifdef HAS_UART
#include "uart_drv.h"
#endif

typedef struct xflowCnt_t {
    uint32_t byte_rx;
    uint32_t byte_tx;
} flowCnt_t;

typedef struct xUartHandle_t {
    volatile bool tx_int;
    volatile bool in_progress;
    volatile bool rx_int;
    bool rx_it_proc_done;
    volatile uint8_t rx_byte;
    volatile uint8_t rx_byte_it;
    volatile uint8_t* rx_buff;
    uint32_t rx_byte_rate;
    uint32_t tx_byte_rate;
    uint32_t rx_byte_rate_min;
    uint32_t tx_byte_rate_min;
    uint32_t rx_byte_rate_max;
    uint32_t tx_byte_rate_max;
    volatile flowCnt_t cnt;
    flowCnt_t cnt_prev;
    uint32_t rx_buff_size;
    uint32_t error_cnt;
    volatile uint32_t tx_cpl_cnt;
    bool init_done;
    uint32_t* base_address;
#ifdef DeviceFamily_CC26X2
    UART_Handle uart_h;
    UART_Params uartParams;
#endif
    volatile uint32_t rx_cnt;
    volatile uint32_t tx_cnt;
#ifdef USE_HAL_DRIVER
    UART_HandleTypeDef uart_h;
#endif /*USE_HAL_DRIVER*/
#ifdef HAS_UART
    bool is_uart_fwd[UART_COUNT];
#endif
    char name[8];
    Fifo_array_t RxFifo;
    Fifo_array_t TxFifo;
} UartHandle_t;

typedef enum { UART_ERROR_PE, UART_ERROR_FE, UART_ERROR_NE, UART_ERROR_ORE } rx_uart_error_t;

#ifdef HAS_UART
extern UartHandle_t huart[UART_COUNT];
bool uart_common_init(uint8_t uart_num);
#endif

bool uart_calc_byte_rate(void);
bool uart_send_banner(uint8_t uart_num, char pattern);
uint32_t calc_uart_transfer_time_ms(uint32_t baudrate, uint32_t bytes);
uint32_t calc_uart_transfer_time_us(uint32_t baudrate, uint32_t bytes);
const char* uart_error2str(rx_uart_error_t e);
#endif /* UART_COMMON_H */
