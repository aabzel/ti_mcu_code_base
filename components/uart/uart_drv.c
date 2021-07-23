#include <stddef.h>
#include <stdint.h>

#include <ti/devices/cc13x2_cc26x2/inc/hw_memmap.h>
#include <ti/devices/cc13x2_cc26x2/inc/hw_ints.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/power/PowerCC26X2.h>
#include <ti/drivers/uart/UARTCC26XX.h>


#include "ti_drivers_config.h"
#include "uart_drv.h"

#ifdef HAS_CLI
#include "uart_string_reader.h"
#endif

UART_Handle uart_0;
UART_Handle uart_1;

char input0;
char input1;

UartHandle_t huart_dbg = {0};
UartHandle_t huart1 = {0};

UARTCC26XX_Object uartCC26XXObjects[CONFIG_UART_COUNT];

static unsigned char uartCC26XXRingBuffer0[64];
static unsigned char uartCC26XXRingBuffer1[32];

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

static void readCallback(UART_Handle handle, char* rxBuf, size_t size) {
    huart_dbg.rx_cnt++;
    huart_dbg.rx_int = true;
    huart_dbg.rx_byte = *(rxBuf);
    //  UART_write(uart, rxBuf, 1);
}

static void writeCallback(UART_Handle handle, void* rxBuf, size_t size) {
    huart_dbg.tx_cnt++;
#ifdef HAS_CLI
    huart_dbg.tx_int = true;
    huart_dbg.tx_cpl_cnt++;
#endif /*HAS_CLI*/
}

static void uart1ReadCallback(UART_Handle handle, char* rxBuf, size_t size) {
    huart1.rx_cnt++;
    huart1.rx_int = true;
    huart1.rx_byte = *(rxBuf);
}

static void uart1WriteCallback(UART_Handle handle, void* rxBuf, size_t size) {
    huart1.tx_cnt++;
    huart1.tx_int = true;
    huart1.tx_cpl_cnt++;
}

static bool init_uart0(void) {
    bool res = false;
    const char echoPrompt[] = "UART0 115200 init ok\r\n";
    UART_Params uart0Params;

    /* Call driver init functions */
    UART_init();

    /* Configure the LED pin */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Create a UART with data processing off. */
    UART_Params_init(&uart0Params);
    uart0Params.baudRate = 115200;
    uart0Params.writeMode = UART_MODE_CALLBACK;
    uart0Params.writeDataMode = UART_DATA_BINARY;
    uart0Params.writeCallback = writeCallback;

    uart0Params.readMode = UART_MODE_CALLBACK;
    uart0Params.readDataMode = UART_DATA_BINARY;
    uart0Params.readCallback = readCallback;

    uart_0 = UART_open(CONFIG_UART_0, &uart0Params);

    if(NULL == uart_0) {
        res = false;
    } else {
        res = true;
    }

    /* Turn on user LED to indicate successful initialization */
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

    UART_write(uart_0, echoPrompt, sizeof(echoPrompt));
    UART_read(uart_0, &input0, 1);
#ifdef HAS_CLI
    huart_dbg.init_done = true;
    huart_dbg.base_address = (uint32_t*)DEBUG_UART;
#endif /*HAS_CLI*/
    return res;
}

static bool init_uart1(void) {
    bool res = false;
    const char echoPrompt[] = "UART1 115200 init ok\r\n";
    UART_Params uart1Params;

    /* Call driver init functions */
    UART_init();

    /* Configure the LED pin */

    /* Create a UART with data processing off. */
    UART_Params_init(&uart1Params);
    uart1Params.baudRate = 115200;
    uart1Params.writeMode = UART_MODE_CALLBACK;
    uart1Params.writeDataMode = UART_DATA_BINARY;
    uart1Params.writeCallback = uart1WriteCallback;

    uart1Params.readMode = UART_MODE_CALLBACK;
    uart1Params.readDataMode = UART_DATA_BINARY;
    uart1Params.readCallback = uart1ReadCallback;

    uart_1 = UART_open(CONFIG_UART_1, &uart1Params);

    if(NULL == uart_1) {
        res = false;
    } else {
        res = true;
    }

    UART_write(uart_1, echoPrompt, sizeof(echoPrompt));
    UART_read(uart_1, &input1, 1);
    return res;
}

bool init_uart(void) {
    bool res = true;
    res = init_uart1() && res;
    res = init_uart0() && res;
    return res;
}

int putchar_uart(int ch) {
    uint32_t init_tx_cnt = huart_dbg.tx_cnt;
    UART_write(uart_0, &ch, 1);
    while(init_tx_cnt == huart_dbg.tx_cnt) {
    }
    return ch;
}

void tune_read_char(void) { UART_read(uart_0, &input0, 1); }

bool uart_send_debug(const uint8_t* tx_buffer, uint16_t len) {
    bool res = true;
    uint32_t init_tx_cnt = huart_dbg.tx_cnt;
    UART_write(uart_0, (uint8_t*)tx_buffer, len);
    while(init_tx_cnt == huart_dbg.tx_cnt) {
    }
    return res;
}

bool uart1_send(const uint8_t* tx_buffer, uint16_t len) {
    bool res = true;
    uint32_t init_tx_cnt = huart1.tx_cnt;
    UART_write(uart_1, (uint8_t*)tx_buffer, len);
    while(init_tx_cnt == huart1.tx_cnt) {
    }
    return res;
}

bool uart_send(uint8_t uart_num, uint8_t* array, uint16_t array_len) {
    bool res = false;
    switch(uart_num) {
    case 0:
        res = uart_send_debug(array, array_len);
        break;
    case 1:
        res = uart1_send(array, array_len) ;
        break;
    default:
        res = false;
        break;
    }
    return res;
}

uint32_t uart_get_baud_rate(uint8_t uart_num, uint16_t* mantissa, uint16_t* fraction, uint8_t* over_sampling) {
    return 0;
}

bool uart_set_baudrate(uint8_t uart_num, uint32_t baudrate) { return false; }
