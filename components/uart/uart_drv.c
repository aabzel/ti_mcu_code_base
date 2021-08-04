#include "uart_drv.h"

#include <stddef.h>
#include <stdint.h>
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

UartHandle_t huart[CONFIG_UART_COUNT] = {0};

UARTCC26XX_Object uartCC26XXObjects[CONFIG_UART_COUNT];

static unsigned char uartCC26XXRingBuffer0[32];
static unsigned char uartCC26XXRingBuffer1[80];

#define RX_ARR0_CNT 1U
#define RX_ARR1_CNT 210U

uint8_t rx_buff0[RX_ARR0_CNT];
uint8_t rx_buff1[RX_ARR1_CNT];

static const UARTCC26XX_HWAttrsV2 uartCC26XXHWAttrs[CONFIG_UART_COUNT] = {
    {.baseAddr = UART0_BASE,
     .intNum = INT_UART0_COMB,
     .intPriority = (~0),
     .swiPriority = 0,
     .powerMngrId = PowerCC26XX_PERIPH_UART0,
     .ringBufPtr = uartCC26XXRingBuffer0,
     .ringBufSize = sizeof(uartCC26XXRingBuffer0),
     .rxPin = IOID_2,
     .txPin = IOID_3,
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
     .rxPin = IOID_0,
     .txPin = IOID_1,
     .ctsPin = PIN_UNASSIGNED,
     .rtsPin = PIN_UNASSIGNED,
     .txIntFifoThr = UARTCC26XX_FIFO_THRESHOLD_1_8,
     .rxIntFifoThr = UARTCC26XX_FIFO_THRESHOLD_4_8,
     .errorFxn = NULL},
};

const UART_Config UART_config[CONFIG_UART_COUNT] = {
    {/* CONFIG_UART_0 */
     .fxnTablePtr = &UARTCC26XX_fxnTable,
     .object = &uartCC26XXObjects[CONFIG_UART_0],
     .hwAttrs = &uartCC26XXHWAttrs[CONFIG_UART_0]},
    {/* CONFIG_UART_1 */
     .fxnTablePtr = &UARTCC26XX_fxnTable,
     .object = &uartCC26XXObjects[CONFIG_UART_1],
     .hwAttrs = &uartCC26XXHWAttrs[CONFIG_UART_1]},
};

const uint_least8_t CONFIG_UART_0_CONST = CONFIG_UART_0;
const uint_least8_t UART_count = CONFIG_UART_COUNT;

static void uart0ReadCallback(UART_Handle handle, char* rxBuf, size_t size) {
    huart[0].rx_cnt++;
    huart[0].rx_int = true;
    huart[0].rx_byte = *(rxBuf);
}

static void uart0WriteCallback(UART_Handle handle, void* rxBuf, size_t size) {
    huart[0].tx_cnt++;
    huart[0].tx_int = true;
    huart[0].tx_cpl_cnt++;
}

static void uart1ReadCallback(UART_Handle handle, char* rxBuf, size_t size) {
    huart[1].rx_cnt++;
    huart[1].rx_int = true;
    huart[1].rx_byte = *(rxBuf);
    // huart[1].rx_buff
}

static void uart1WriteCallback(UART_Handle handle, void* rxBuf, size_t size) {
    huart[1].tx_cnt++;
    huart[1].tx_int = true;
    huart[1].tx_cpl_cnt++;
}

static bool init_uart0(void) {
    bool res = false;
    memset(&huart[0], 0x00, sizeof(huart[0]));
    const char echoPrompt[] = "UART0 115200 init ok\r\n";
    UART_Params uart0Params;
    huart[0].rx_cnt = 0;
    huart[0].tx_cnt = 0;
    huart[0].tx_cpl_cnt = 0;
    huart[0].tx_byte_cnt = 0;
    huart[1].rx_buff = rx_buff0;
    strncpy(huart[0].name, "CLI", sizeof(huart[0].name));
    /* Call driver init functions */
    UART_init();

    /* Configure the LED pin */

    /* Create a UART with data processing off. */
    UART_Params_init(&uart0Params);
    uart0Params.baudRate = UART0_BAUD_RATE;
    uart0Params.writeMode = UART_MODE_CALLBACK;
    uart0Params.writeDataMode = UART_DATA_BINARY;
    uart0Params.writeCallback = uart0WriteCallback;

    uart0Params.readMode = UART_MODE_CALLBACK;
    uart0Params.readDataMode = UART_DATA_BINARY;
    uart0Params.readCallback = uart0ReadCallback;

    huart[0].uart_h = UART_open(CONFIG_UART_0, &uart0Params);

    if(NULL == huart[0].uart_h) {
        res = false;
    } else {
        res = true;
    }

    /* Turn on user LED to indicate successful initialization */

    UART_write(huart[0].uart_h, echoPrompt, sizeof(echoPrompt));
    UART_read(huart[0].uart_h, &huart[0].rx_byte, 1);
    huart[0].init_done = true;
    huart[0].base_address = (uint32_t*)DEBUG_UART;
    return res;
}

static bool init_uart1(void) {
    bool res = false;
    memset(&huart[1], 0x00, sizeof(huart[1]));
    huart[1].rx_cnt = 0;
    huart[1].tx_cnt = 0;
    huart[1].tx_cpl_cnt = 0;
    huart[1].tx_byte_cnt = 0;
    huart[1].rx_buff = rx_buff1;
    strncpy(huart[1].name, "Ublox", sizeof(huart[1].name));
    const char echoPrompt[] = "UART1 init ok\r\n"; //
    UART_Params uart1Params;

    /* Call driver init functions */
    UART_init();

    /* Configure the LED pin */

    /* Create a UART with data processing off. */
    UART_Params_init(&uart1Params);
    uart1Params.baudRate = UART1_BAUD_RATE;
    uart1Params.writeMode = UART_MODE_CALLBACK;
    uart1Params.writeDataMode = UART_DATA_BINARY;
    uart1Params.writeCallback = uart1WriteCallback;

    uart1Params.readMode = UART_MODE_CALLBACK;
    uart1Params.readDataMode = UART_DATA_BINARY;
    uart1Params.readCallback = uart1ReadCallback;

    huart[1].uart_h = UART_open(CONFIG_UART_1, &uart1Params);

    if(NULL == huart[1].uart_h) {
        res = false;
    } else {
        res = true;
    }

    UART_write(huart[1].uart_h, echoPrompt, sizeof(echoPrompt));
    UART_read(huart[1].uart_h, huart[1].rx_buff, RX_ARR1_CNT);
    return res;
}

bool uart_init(void) {
    bool res = true;
    res = init_uart1() && res;
    res = init_uart0() && res;
    return res;
}

int cli_putchar_uart(int ch) {
    uint32_t init_tx_cnt = huart[0].tx_cnt;
    UART_write(huart[0].uart_h, &ch, 1);
    while(init_tx_cnt == huart[0].tx_cnt) {
    }
    return ch;
}

void cli_tune_read_char(void) { UART_read(huart[0].uart_h, &huart[0].rx_byte, 1); }

bool uart_send_ll(uint8_t uart_num, const uint8_t* tx_buffer, uint16_t len) {
    bool res = true;
    uint32_t time_out = 0;
    uint32_t init_tx_cnt = huart[uart_num].tx_cnt;
    UART_write(huart[uart_num].uart_h, (uint8_t*)tx_buffer, len);
    while(init_tx_cnt == huart[uart_num].tx_cnt) {
        time_out++;
        if(10000000 < time_out) {
            res = false;
            break;
        }
    }
    return res;
}

bool uart_send(uint8_t uart_num, uint8_t* array, uint16_t array_len) {
    bool res = false;
    if(uart_num < CONFIG_UART_COUNT) {
        res = uart_send_ll(uart_num, array, array_len);
    }
    return res;
}

uint32_t uart_get_baud_rate(uint8_t uart_num, uint16_t* mantissa, uint16_t* fraction, uint8_t* over_sampling) {
    return 0;
}

bool uart_set_baudrate(uint8_t uart_num, uint32_t baudrate) { return false; }

bool proc_uart(uint8_t uart_index) {
    bool res = false;
    if(true == huart[uart_index].rx_int) {
        huart[uart_index].rx_int = false;
        if(1 == uart_index) {
            uint16_t i;
            uint8_t rx_byte;
            for(i = 0; i < RX_ARR1_CNT; i++) {
                rx_byte = huart[uart_index].rx_buff[i];
#ifdef HAS_NMEA
                nmea_proc_byte(rx_byte);
#endif /*HAS_NMEA*/
#ifdef HAS_UBLOX
                ubx_proc_byte(rx_byte);
#endif /*HAS_UBLOX*/
            }
            UART_read(huart[uart_index].uart_h, &huart[uart_index].rx_buff[0], RX_ARR1_CNT);
        } else if(0 == uart_index) {
            UART_read(huart[uart_index].uart_h, &huart[uart_index].rx_byte, 1);
        }
        res = true;
    }
    return res;
}

bool proc_uart1(void) { return proc_uart(1); }
