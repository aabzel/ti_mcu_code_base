#include "rs232_drv.h"


#include <stdbool.h>
#include <stdint.h>
#include <gpio.h>
#include <GPIO.h>

#include "board_layout.h"
#include "uart_drv.h"
#include "system.h"
#ifdef HAS_TBFP
#include "tbfp_protocol.h"
#endif /*HAS_TBFT*/

bool rs232_send(uint8_t* const data, uint32_t len){
    bool res = false;
    GPIO_writeDio(DIO_PS_RS232, 1);
    res = uart_send(UART_NUM_RS232, (uint8_t*)data, len, true);
    return res;
}

bool rs232_init(void) {
    bool res = true;
#ifdef HAS_RS232
    GPIO_setConfig(CONF_GPIO_PS_RS232, gpio_get_cfg_dio(DIO_PS_RS232));
    GPIO_writeDio(DIO_PS_RS232, 1);
#endif /*HAS_RS232*/
    return res;
}

bool proc_rs232(void){
    bool res = false;
#ifdef HAS_TBFP
    /*HeartBeat RS232 Frame*/
    res = tbfp_send_ping(FRAME_ID_PONG, IF_RS232);
#endif /*HAS_TBFP*/
    return res;
}
