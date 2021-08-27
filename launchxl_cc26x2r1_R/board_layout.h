#ifndef BOARD_LAYOUT_H
#define BOARD_LAYOUT_H

#include <ti/drivers/GPIO.h>
#include <ti/drivers/PIN.h>

#define SX1262_SS_DIO_NO 11
#define SX1262_RST_DIO_NO 27
#define SX1262_BUSY_DIO_NO 29
#define SX1262_INT_DIO_NO 28

#define DIO_CNT 31

#define PIN_NAME_LEN 10
typedef struct xPin_t {
    uint8_t dio;
    uint8_t mcu_pin;
    uint8_t aux_pin;
    char name[PIN_NAME_LEN];
    GPIO_CallbackFxn callBack;
    GPIO_PinConfig pinConfig;
    PIN_Config pinCfg;
} Pin_t;


extern const Pin_t PinTable[DIO_CNT];

#endif /* BOARD_LAYOUT_H  */
