#include "uart_drv.h"

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
#endif

#ifdef HAS_UBLOX
#include "ubx_protocol.h"
#endif

#ifdef HAS_NMEA
#include "nmea_protocol.h"
#endif

#ifdef HAS_CLI
#include "uart_string_reader.h"
#endif

#include "sys_config.h"

UartHandle_t huart[UART_COUNT] = {0};

UARTCC26XX_Object uartCC26XXObjects[UART_COUNT];

static unsigned char uartCC26XXRingBuffer0[32];
static unsigned char uartCC26XXRingBuffer1[160];

#define RX_ARR0_CNT 1U
#define RX_ARR1_CNT 210U

uint8_t rx_buff[UART_COUNT][RX_ARR0_CNT];

static const UARTCC26XX_HWAttrsV2 uartCC26XXHWAttrs[UART_COUNT] = {
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
};

const UART_Config UART_config[UART_COUNT] = {
    {.fxnTablePtr = &UARTCC26XX_fxnTable, .object = &uartCC26XXObjects[0], .hwAttrs = &uartCC26XXHWAttrs[0]},
    {.fxnTablePtr = &UARTCC26XX_fxnTable, .object = &uartCC26XXObjects[1], .hwAttrs = &uartCC26XXHWAttrs[1]},
};

const uint_least8_t UART_count = UART_COUNT;

static void uart0ReadCallback(UART_Handle handle, char* rx_buf, size_t size) {
    huart[0].rx_cnt++;
    huart[0].rx_int = true;
    if((1 == size) && (NULL!=rx_buf)) {
       huart[0].rx_byte_it = (uint8_t) *(rx_buf);
    } else {
       huart[0].error_cnt++;
    }
}

static void uart0WriteCallback(UART_Handle handle, void* rxBuf, size_t size) {
    huart[0].tx_cnt++;
    huart[0].tx_int = true;
    huart[0].tx_cpl_cnt++;
}

static void uart1ReadCallback(UART_Handle handle, char* rx_buf, size_t size) {
    huart[1].rx_cnt++;
    huart[1].rx_int = true;
    if(rx_buf){
        huart[1].rx_byte = *(rx_buf);
    }
}

static void uart1WriteCallback(UART_Handle handle, void* rxBuf, size_t size) {
    huart[1].tx_cnt++;
    huart[1].tx_int = true;
    huart[1].tx_cpl_cnt++;
}

static const uint32_t uartNum2Base[UART_COUNT] = {UART0_BASE, UART1_BASE};

bool uart_send_ll(uint8_t uart_num, const uint8_t* tx_buffer, uint16_t len) {
    bool res = true;
    uint32_t time_out = 0;
    uint32_t init_tx_cnt = huart[uart_num].tx_cnt;
    UART_write(huart[uart_num].uart_h, (uint8_t*)tx_buffer, len);
    /*TODO Calc needed time to wait*/
    while(init_tx_cnt == huart[uart_num].tx_cnt) {
        time_out++;
        if(10000 < time_out) {/*TODO find val*/
            res = false;
            huart[uart_num].error_cnt++;
            break;
        }
    }
    return res;
}

bool uart_send(uint8_t uart_num, uint8_t* array, uint16_t array_len) {
    bool res = false;
    if(1==uart_num){
#ifdef HAS_BOOTLOADER
        res = false;
        return res;
#endif /*HAS_BOOTLOADER*/
    }
    if(uart_num < UART_COUNT) {
        res = uart_send_ll(uart_num, array, array_len);
    }
    return res;
}

int cli_putchar_uart(int character) {
    int out_ch = 0;
    /*Works on little endian*/
    bool res = uart_send(CLI_UART_NUM, (uint8_t*)&character, 1);
    if(res) {
        out_ch = character;
    }

    return out_ch;
}
bool uart_read(uint8_t uart_num, uint8_t* out_array, uint16_t array_len) {
    bool res = false;
    if(uart_num < UART_COUNT) {
        int_fast32_t ret=0;
        ret = UART_read(huart[uart_num].uart_h, out_array, array_len);
        if(0<ret){
           res = true;
        }
    }

    return res;
}
#if 0
void cli_tune_read_char(void) { /*uart_read(CLI_UART_NUM, &huart[CLI_UART_NUM].rx_byte, 1);*/ }
#endif


uint32_t uart_get_baud_rate(uint8_t uart_num, uint16_t* mantissa, uint16_t* fraction, uint8_t* over_sampling) {
    return 0;
}

bool uart_set_baudrate(uint8_t uart_num, uint32_t baudrate) { return false; }

bool proc_uart(uint8_t uart_index) {
    bool res = false;
    if(true == huart[uart_index].rx_int) {
        huart[uart_index].rx_int = false;
        if(1 == uart_index) {
            uint16_t byte_no;
#if defined(HAS_NMEA) || defined(HAS_UBLOX)
            uint8_t rx_byte = 0;
#endif /*defined(HAS_NMEA) ||defined(HAS_UBLOX) */
            for(byte_no = 0; byte_no < RX_ARR1_CNT; byte_no++) {
#if defined(HAS_NMEA) || defined(HAS_UBLOX)
                rx_byte = huart[uart_index].rx_buff[byte_no];
#endif /*defined(HAS_NMEA) ||defined(HAS_UBLOX) */

#ifdef HAS_NMEA
                nmea_proc_byte(rx_byte);
#endif /*HAS_NMEA*/

#ifdef HAS_UBLOX
                ubx_proc_byte(rx_byte);
#endif /*HAS_UBLOX*/
            }
            res = uart_read(uart_index, &huart[uart_index].rx_buff[0], RX_ARR1_CNT);
        } else if(0 == uart_index) {
            huart[uart_index].rx_byte = 0xFF;
            res = uart_read(uart_index, &huart[uart_index].rx_byte, 1);
            if(0x00 != huart[uart_index].rx_byte) {
                res = true;
            } else {
                res = false;
            }
        }
        res = true;
    }
    return res;
}
#define UNLIKELY_SYMBOL1 0XFF
#define UNLIKELY_SYMBOL2 0X00

static bool uart_poll(uint8_t uart_index) {
    /*In case of uart interrupts fail*/
    bool res = true;
    static uint8_t rx_byte = UNLIKELY_SYMBOL1;
    res = uart_read(uart_index, &rx_byte, 1);
    if((UNLIKELY_SYMBOL1 != rx_byte) &&(UNLIKELY_SYMBOL2 != rx_byte) &&(res)) {
        res = true;
        huart[uart_index].rx_byte = rx_byte;
        if(CLI_UART_NUM == uart_index) {
            uart_string_reader_rx_callback(&cmd_reader, (char)rx_byte);
        }
    } else {
        res = false;
    }
    return res;
}


bool proc_uarts(void) {
    bool res = false;
    res = uart_poll(0);

#ifdef HAS_UART1
    res = proc_uart(1);
#endif /*HAS_UART1*/
    return res;
}


static bool init_uart_ll(uint8_t uart_num, char* in_name, uint32_t baud_rate) {
    bool res = false;
    if(uart_num < UART_COUNT) {
        memset(&huart[uart_num], 0x00, sizeof(huart[uart_num]));
        huart[uart_num].rx_cnt = 0;
        huart[uart_num].tx_cnt = 0;
        huart[uart_num].tx_cpl_cnt = 0;
        huart[uart_num].tx_byte_cnt = 0;
        huart[uart_num].rx_buff = &rx_buff[uart_num][0];
        strncpy(huart[uart_num].name, in_name, sizeof(huart[uart_num].name));
        char connectionHint[40] = "";
        snprintf(connectionHint, sizeof(connectionHint), "UART%u %u\n\r", uart_num, baud_rate);

        UART_init();

        UART_Params_init(&huart[uart_num].uartParams);
        huart[uart_num].uartParams.baudRate = baud_rate;
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
            huart[uart_num].uartParams.writeCallback = (UART_Callback)uart1WriteCallback;
            huart[uart_num].uartParams.readCallback = (UART_Callback)uart1ReadCallback;
        } else {
            huart[uart_num].uartParams.writeCallback = (UART_Callback)NULL;
            huart[uart_num].uartParams.readCallback = (UART_Callback)NULL;
        }

        huart[uart_num].uart_h = UART_open(uart_num, &huart[uart_num].uartParams);
        if(NULL == huart[uart_num].uart_h) {
            res = false;
        } else {
            res = true;
            huart[uart_num].base_address = (uint32_t*)uartNum2Base[uart_num];
            huart[uart_num].init_done = true;
            huart[uart_num].rx_it_proc_done = true;
#ifdef UART_SHOW
            res = uart_send(uart_num, (uint8_t*)connectionHint, strlen(connectionHint));
#endif /*UART_SHOW*/
            uart_read(uart_num, &huart[uart_num].rx_byte, 1);
        }
    }
    return res;
}

bool uart_init(void) {
    bool res = true;
#ifdef HAS_GENERIC
    res = init_uart_ll(1, "ZedF9P", UART1_BAUD_RATE) && res;
#ifdef HAS_UBLOX
#endif /*HAS_UBLOX*/
#endif /*HAS_GENERIC*/
    res = init_uart_ll(0, "CLI", UART0_BAUD_RATE) && res;
    return res;
}
