#include <stddef.h>
#include <stdint.h>

#include <ti/devices/cc13x2_cc26x2/inc/hw_memmap.h>
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
char input;
UartHandle_t huart_dbg = {0};

UARTCC26XX_Object uartCC26XXObjects[CONFIG_UART_COUNT];

static unsigned char uartCC26XXRingBuffer0[32];

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
};

const UART_Config UART_config[CONFIG_UART_COUNT] = {
    {/* CONFIG_UART_0 */
     .fxnTablePtr = &UARTCC26XX_fxnTable,
     .object = &uartCC26XXObjects[CONFIG_UART_0],
     .hwAttrs = &uartCC26XXHWAttrs[CONFIG_UART_0]},
};

const uint_least8_t CONFIG_UART_0_CONST = CONFIG_UART_0;
const uint_least8_t UART_count = CONFIG_UART_COUNT;


// Read callback function
static void readCallback(UART_Handle handle, char *rxBuf, size_t size){
  huart_dbg.rx_cnt++  ;
  huart_dbg.rx_int = true;
  huart_dbg.rx_byte = *(rxBuf);
//  UART_write(uart, rxBuf, 1);
}

// Write callback function
static void writeCallback(UART_Handle handle, void *rxBuf, size_t size){
    huart_dbg.tx_cnt++;
#ifdef HAS_CLI
    huart_dbg.tx_int = true;
    huart_dbg.tx_cpl_cnt++;
#endif /*HAS_CLI*/
}


void init_uart(void) {
    const char echoPrompt[] = "UART0 115200 init ok\r\n";
    UART_Params uartParams;

    /* Call driver init functions */
    UART_init();

    /* Configure the LED pin */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Create a UART with data processing off. */
    UART_Params_init(&uartParams);
    uartParams.baudRate = 115200;
    uartParams.writeMode     = UART_MODE_CALLBACK;
    uartParams.writeDataMode = UART_DATA_BINARY;
	uartParams.writeCallback = writeCallback;

    uartParams.readMode     = UART_MODE_CALLBACK;
    uartParams.readDataMode = UART_DATA_BINARY;
	uartParams.readCallback  = readCallback;

	uart_0 = UART_open(CONFIG_UART_0, &uartParams);

    if(uart_0 == NULL) {
        /* UART_open() failed */
        while(1)
            ;
    }

    /* Turn on user LED to indicate successful initialization */
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

    UART_write(uart_0, echoPrompt, sizeof(echoPrompt));
    UART_read(uart_0, &input, 1);
#ifdef HAS_CLI
    huart_dbg.init_done = true;
    huart_dbg.base_address = (uint32_t*)DEBUG_UART;
#endif /*HAS_CLI*/

}

int putchar_uart(int ch) {
  uint32_t init_tx_cnt=huart_dbg.tx_cnt;
  UART_write(uart_0, &ch, 1);
  while(init_tx_cnt==huart_dbg.tx_cnt){

  }
  return ch;
}

void tune_read_char(void){
    UART_read(uart_0, &input, 1);
}

void proc_uart(void) {
   //
    //UART_write(uart, &input, 1);
}


bool uart_send_debug(const uint8_t *tx_buffer, uint16_t len) {
  bool res = true;
  uint32_t init_tx_cnt=huart_dbg.tx_cnt;
  UART_write(uart_0, (uint8_t *)tx_buffer, len);
  while(init_tx_cnt==huart_dbg.tx_cnt){

  }
  return res;
}


