#include "uart_drv.h"

#include <gpio.h>
#include <ioc.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef DeviceFamily_CC26X2
#include <ti/devices/cc13x2_cc26x2/inc/hw_ints.h>
#include <ti/devices/cc13x2_cc26x2/inc/hw_memmap.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/power/PowerCC26X2.h>
#include <ti/drivers/uart/UARTCC26XX.h>
#endif /*DeviceFamily_CC26X2*/

#include "clocks.h"

#ifdef HAS_GPIO
#include "gpio_drv.h"
#endif /*HAS_GPIO*/

#ifdef HAS_UBLOX
#include "ubx_protocol.h"
#endif /*HAS_UBLOX*/

#ifdef HAS_NMEA
#include "nmea_protocol.h"
#endif /*HAS_NMEA*/

#ifdef HAS_RTCM3
#include "rtcm3_protocol.h"
#endif /*HAS_RTCM3*/

#ifdef HAS_CLI
#include "uart_string_reader.h"
#endif /*HAS_CLI*/

#include "fifo_char.h"
#include "sys_config.h"
#include "uart_common.h"

UARTCC26XX_Object uartCC26XXObjects[UART_COUNT];

static unsigned char uartCC26XXRingBuffer0[32];

#ifdef HAS_UART1
static unsigned char uartCC26XXRingBuffer1[160];
#endif /*HAS_UART1*/

#define RX_ARR0_CNT 1U
#define RX_ARR1_CNT 210U

static volatile uint8_t rx_buff[UART_COUNT][UART_FIFO_RX_SIZE]; /*TODO Make variable array size for each UART*/
static volatile uint8_t tx_buff[UART_COUNT][UART_FIFO_TX_SIZE];

const UARTCC26XX_HWAttrsV2 uartCC26XXHWAttrs[UART_COUNT] = {
    {.baseAddr = UART0_BASE,
     .intNum = INT_UART0_COMB,
     .intPriority = (~0),
     .swiPriority = 0,
     .powerMngrId = PowerCC26XX_PERIPH_UART0,
     .ringBufPtr = uartCC26XXRingBuffer0,
     .ringBufSize = sizeof(uartCC26XXRingBuffer0),
     .rxPin = DIO_UART_CLI_RX,
     .txPin = DIO_UART_CLI_TX,
     .ctsPin = PIN_UNASSIGNED,
     .rtsPin = PIN_UNASSIGNED,
     .txIntFifoThr = UARTCC26XX_FIFO_THRESHOLD_1_8,
     .rxIntFifoThr = UARTCC26XX_FIFO_THRESHOLD_4_8,
     .errorFxn = NULL},
#ifdef HAS_UART1
    {.baseAddr = UART1_BASE,
     .intNum = INT_UART1_COMB,
     .intPriority = (~0),
     .swiPriority = 0,
     .powerMngrId = PowerCC26XX_PERIPH_UART1,
     .ringBufPtr = uartCC26XXRingBuffer1,
     .ringBufSize = sizeof(uartCC26XXRingBuffer1),
#ifdef HAS_UBLOX
     .rxPin = DIO_GNSS_TXD,
     .txPin = DIO_GNSS_RXD,
#else
     .rxPin = 10,
     .txPin = 11,
#endif /*HAS_UBLOX*/
     .ctsPin = PIN_UNASSIGNED,
     .rtsPin = PIN_UNASSIGNED,
     .txIntFifoThr = UARTCC26XX_FIFO_THRESHOLD_1_8,
     .rxIntFifoThr = UARTCC26XX_FIFO_THRESHOLD_4_8,
     .errorFxn = NULL},
#endif /*HAS_UART1*/
};

const UART_Config UART_config[UART_COUNT] = {
    {.fxnTablePtr = &UARTCC26XX_fxnTable, .object = &uartCC26XXObjects[0], .hwAttrs = &uartCC26XXHWAttrs[0]},
#ifdef HAS_UART1
    {.fxnTablePtr = &UARTCC26XX_fxnTable, .object = &uartCC26XXObjects[1], .hwAttrs = &uartCC26XXHWAttrs[1]},
#endif /*HAS_UART1*/
};

const uint_least8_t UART_count = UART_COUNT;
static uint32_t baudRateLuTable[UART_COUNT] = {UART0_BAUD_RATE,
#ifdef HAS_UART1
                                               UART1_BAUD_RATE
#endif
};

static void uart0ReadCallback(UART_Handle handle, char* rx_buf, size_t size) {
    huart[0].rx_cnt++;
    huart[0].rx_it_proc_done = false;
    huart[0].rx_int = true;
    if((1 == size) && (NULL != rx_buf)) {
        bool res = false;
        huart[0].rx_byte_it = (uint8_t) * (rx_buf);
        res = fifo_push(&huart[0].RxFifo, huart[0].rx_byte);
        if(false == res) {
            huart[0].error_cnt++;
        }
#ifdef HAS_UART0_FWD
        if(true == huart[0].is_uart_fwd[1]) {
            res = fifo_push(&huart[1].TxFifo, huart[0].rx_byte);
            if(false == res) {
                huart[1].error_cnt++;
            }
        }
#endif /*HAS_UART0_FWD*/
    } else {
        huart[0].error_cnt++;
    }
}
#ifdef HAS_UART1
static uint8_t ch1 = 0;
static void uart1ReadCallback(UART_Handle handle, char* rx_buf, size_t size) {
    huart[1].rx_cnt++;
    huart[1].rx_int = true;
    huart[1].rx_it_proc_done = false;
    if(NULL != rx_buf) {
        huart[1].rx_byte = *(rx_buf);
        bool res = false;
        res = fifo_push(&huart[1].RxFifo, huart[1].rx_byte);
        if(false == res) {
            huart[1].error_cnt++;
        }
#ifdef HAS_UART1_FWD
        if(true == huart[1].is_uart_fwd[0]) {
            res = fifo_push(&huart[0].TxFifo, huart[1].rx_byte);
            if(false == res) {
                huart[0].error_cnt++;
            }
        }
#endif /*HAS_UART1_FWD*/
    }
    uart_read(1, &ch1, 1);
}
#endif /*HAS_UART1*/

static void uart0WriteCallback(UART_Handle handle, void* rxBuf, size_t size) {
    huart[0].tx_cnt++;
    huart[0].tx_int = true;
    huart[0].tx_cpl_cnt++;
}
#ifdef HAS_UART1
static void uart1WriteCallback(UART_Handle handle, void* rxBuf, size_t size) {
    huart[1].tx_cnt++;
    huart[1].tx_int = true;
    huart[1].tx_cpl_cnt++;
}
#endif /*HAS_UART1*/
static const uint32_t uartNum2Base[UART_COUNT] = {UART0_BASE,
#ifdef HAS_UART1
                                                  UART1_BASE
#endif /*HAS_UART1*/
};

bool is_uart_valid(uint8_t uart_num) {
    bool res = false;
    if(uart_num < UART_COUNT) {
        res = true;
    }
    return res;
}

bool uart_send_wait_ll(uint8_t uart_num, const uint8_t* tx_buffer, uint16_t len, bool is_wait) {
    bool res = true;
    uint32_t init_tx_cnt = huart[uart_num].tx_cnt;
    uint32_t baudrate = uart_get_baudrate(uart_num);
    uint32_t up_time_start_ms = get_time_ms32();
    uint32_t up_time_cur_ms = 0;
    uint32_t duration_ms = 0;
    uint32_t tx_duration_ms = calc_uart_transfer_time_ms(baudrate, len);
    /*TODO Wait previous transfer*/
    UART_write(huart[uart_num].uart_h, (uint8_t*)tx_buffer, len);
    /*TODO Calc needed time to wait*/
    if(is_wait) {
        while(init_tx_cnt == huart[uart_num].tx_cnt) {
            up_time_cur_ms = get_time_ms32();
            duration_ms = up_time_cur_ms - up_time_start_ms;
            if((10 * (tx_duration_ms + 1)) < duration_ms) { /*TODO find val*/
                res = false;
                huart[uart_num].error_cnt++;
                break;
            }
        }
    }
    return res;
}

bool uart_send(uint8_t uart_num, uint8_t* array, uint16_t array_len, bool is_wait) {
    bool res = false;
    if(1 == uart_num) {
#ifdef HAS_BOOTLOADER
        res = false;
        return res;
#endif /*HAS_BOOTLOADER*/
    }
    if(uart_num < UART_COUNT) {
        res = uart_send_wait_ll(uart_num, array, array_len, is_wait);
    }
    return res;
}

int cli_putchar_uart(int character) {
    int out_ch = 0;
    /*Works on little endian*/
    bool res = uart_send(UART_NUM_CLI, (uint8_t*)&character, 1, true);
    if(res) {
        out_ch = character;
    }

    return out_ch;
}

bool uart_read(uint8_t uart_num, uint8_t* out_array, uint16_t array_len) {
    bool res = false;
    if(uart_num < UART_COUNT) {
        int_fast32_t ret = 0;
        ret = UART_read(huart[uart_num].uart_h, out_array, array_len);
        if(0 <= ret) {
            res = true;
        }
    }

    return res;
}

uint32_t uart_get_baud_rate(uint8_t uart_num, uint16_t* mantissa, uint16_t* fraction, uint8_t* over_sampling) {
    return 0;
}

bool uart_set_baudrate(uint8_t uart_num, uint32_t baudrate) {
    /*TODO: Implement set baud rate*/
    return false;
}

uint32_t uart_get_baudrate(uint8_t uart_num) {
    uint32_t baudrate = 0;
    if(uart_num < UART_COUNT) {
        baudrate = baudRateLuTable[uart_num];
    }
    return baudrate;
}

bool proc_uart(uint8_t uart_index) {
    bool res = false;

    // huart[uart_index].rx_int = false;
    if(1 == uart_index) {
        uint8_t rx_byte = 0;
        res = true;
        bool loop = true;
        uint32_t cnt = 0;
        while(loop) {
            res = fifo_pull(&huart[1].RxFifo, (char*)&rx_byte);
            if(true == res) {
                loop = true;
#ifdef HAS_RTCM3
                rtcm3_proc_byte(&Rtcm3Porotocol[RT_UART_ID], rx_byte);
#endif /*HAS_RTCM3*/

#ifdef HAS_NMEA
                nmea_proc_byte(rx_byte);
#endif /*HAS_NMEA*/

#ifdef HAS_UBLOX
                ubx_proc_byte(rx_byte);
#endif /*HAS_UBLOX*/
            } else {
                loop = false;
            }
            cnt++;
            if((UART_FIFO_RX_SIZE * 2) < cnt) {
                loop = false;
            }
        } /*while(loop) */
        res = true;
    } else if(0 == uart_index) {
        res = true;
    } else {
        res = false;
    }

    return res;
}

#define UNLIKELY_SYMBOL1 0XFF
#define UNLIKELY_SYMBOL2 0X00

static bool uart_poll(uint8_t uart_index) {
    /*In case of uart interrupts fail*/
    bool res = true;
    static uint8_t rx_byte = UNLIKELY_SYMBOL1;
    if(true == huart[UART_NUM_CLI].rx_it_proc_done) {
        res = uart_read(uart_index, &rx_byte, 1);
        if((UNLIKELY_SYMBOL1 != rx_byte) && (UNLIKELY_SYMBOL2 != rx_byte) && (res)) {
            res = true;
            huart[uart_index].rx_byte = rx_byte;
            if(UART_NUM_CLI == uart_index) {
                // uart_string_reader_rx_callback(&cmd_reader, (char)rx_byte);
            }
        } else {
            res = false;
        }
    }
    return res;
}

bool proc_uarts(void) {
    bool res = false;
    uint32_t uart_error = 0;
    uart_error = UARTRxErrorGet(uartCC26XXHWAttrs[0].baseAddr); // 9
    if(uart_error) {
        res = uart_poll(0);
    }
#ifdef HAS_UART1
    uart_error = UARTRxErrorGet(uartCC26XXHWAttrs[1].baseAddr); // 9
    if(uart_error) {
        res = uart_poll(1);
    }
#endif /*HAS_UART1*/

#ifdef HAS_UART1
    res = proc_uart(1);
#endif /*HAS_UART1*/
    return res;
}

#ifdef HAS_UART_FWD
static bool proc_uart_fwd_ll(uint8_t uart_num) {
    bool res = false;
    fifo_index_t read_size = 0;
    char txData[UART_FIFO_TX_SIZE * 2] = {0};
    res = fifo_pull_array(&huart[uart_num].TxFifo, txData, &read_size);
    if((true == res) && (0 < read_size)) {
        res = uart_send(uart_num, (uint8_t*)txData, read_size, false);
    } else {
    }
    return res;
}
#endif

#ifdef HAS_UART0_FWD
bool proc_uart0_fwd(void) {
    bool res =  proc_uart_fwd_ll(0);
    return res;
}
#endif /*HAS_UART0_FWD*/

#ifdef HAS_UART1_FWD
bool proc_uart1_fwd(void) {
    bool res =  proc_uart_fwd_ll(1);
    return res;
}
#endif /*HAS_UART1_FWD*/


static bool init_uart_ll(uint8_t uart_num, char* in_name) {
    bool res = false;
    if(uart_num < UART_COUNT) {
        memset(&huart[uart_num], 0x00, sizeof(huart[uart_num]));
        huart[uart_num].rx_buff = &rx_buff[uart_num][0];
        strncpy(huart[uart_num].name, in_name, sizeof(huart[uart_num].name));
#ifdef UART_SHOW
        char connectionHint[40] = "";
        snprintf(connectionHint, sizeof(connectionHint), "UART%u %u\n\r", uart_num, uart_get_baudrate(uart_num));
#endif
        UART_init();

        UART_Params_init(&huart[uart_num].uartParams);
        huart[uart_num].uartParams.baudRate = uart_get_baudrate(uart_num);
        huart[uart_num].uartParams.writeMode = UART_MODE_CALLBACK;
        huart[uart_num].uartParams.writeDataMode = UART_DATA_BINARY;
        huart[uart_num].uartParams.readMode = UART_MODE_CALLBACK;
        huart[uart_num].uartParams.readDataMode = UART_DATA_BINARY;
        huart[uart_num].uartParams.writeCallback = (UART_Callback)NULL;
        huart[uart_num].uartParams.readCallback = (UART_Callback)NULL;
        if(0 == uart_num) {
            huart[uart_num].uartParams.writeCallback = (UART_Callback)uart0WriteCallback;
            huart[uart_num].uartParams.readCallback = (UART_Callback)uart0ReadCallback;
        } else if(1 == uart_num) {
#ifdef HAS_UART1
            huart[uart_num].uartParams.writeCallback = (UART_Callback)uart1WriteCallback;
            huart[uart_num].uartParams.readCallback = (UART_Callback)uart1ReadCallback;
#endif
        } else {
            huart[uart_num].uartParams.writeCallback = (UART_Callback)NULL;
            huart[uart_num].uartParams.readCallback = (UART_Callback)NULL;
        }

        huart[uart_num].uart_h = UART_open(uart_num, &huart[uart_num].uartParams);
        if(NULL == huart[uart_num].uart_h) {
            res = false;
        } else {
            res = fifo_init(&huart[uart_num].TxFifo, (char*)&tx_buff[uart_num][0], UART_FIFO_TX_SIZE);
            res = fifo_init(&huart[uart_num].RxFifo, (char*)&rx_buff[uart_num][0], UART_FIFO_RX_SIZE) && res;
            huart[uart_num].base_address = (uint32_t*)uartNum2Base[uart_num];
            huart[uart_num].init_done = true;
            huart[uart_num].rx_it_proc_done = true;
#ifdef UART_SHOW
            res = uart_send(uart_num, (uint8_t*)connectionHint, strlen(connectionHint));
#endif /*UART_SHOW*/
            uart_read(uart_num, (uint8_t*)&huart[uart_num].rx_byte, 1);
        }
    }
    return res;
}

bool uart_init(void) {
    bool res = true;
#ifdef HAS_GENERIC
#ifdef HAS_UBLOX
    res = init_uart_ll(1, "ZedF9P") && res;
#endif /*HAS_UBLOX*/
#endif /*HAS_GENERIC*/
    res = init_uart_ll(0, "CLI") && res;
    return res;
}

bool uart_deinit(uint8_t uart_num) {
    bool res = false;
    res = is_uart_valid(uart_num);
    if(res) {
        res = gpio_set_dir(uartCC26XXHWAttrs[uart_num].txPin, GPIO_DIR_NONE) & res;
        res = gpio_set_dir(uartCC26XXHWAttrs[uart_num].rxPin, GPIO_DIR_NONE) & res;
        IOCIOPortIdSet(uartCC26XXHWAttrs[uart_num].txPin, IOC_PORT_GPIO);
        IOCIOPortIdSet(uartCC26XXHWAttrs[uart_num].rxPin, IOC_PORT_GPIO);
    }

    return res;
}
