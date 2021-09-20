#ifndef INIT_GPIO_H
#define INIT_GPIO_H

#include <gpio.h>
#include <hw_memmap.h>
#include <stdbool.h>
#include <stdint.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/PIN.h>

#include "board_layout.h"

#define IOC_BASE 0x40081000


#define CONFIG_GPIO_LED_0 0
#define CONFIG_GPIO_LED_1 1
#ifdef HAS_SX1262
#define CONFIG_GPIO_LORA_CS 2
#define CONFIG_GPIO_LORA_RST 3
#define CONFIG_GPIO_LORA_INT 4
#define CONFIG_GPIO_LORA_BSY 5
#define LORA_PINS_CNT 4
#else
#define LORA_PINS_CNT 0
#endif /*HAS_SX1262*/
//#define CONFIG_GPIO_LOOP_SENSOR 6
#ifdef LAUNCHXL_CC26X2R1
#define CONFIG_GPIO_BUTTON_0 6
#define CONFIG_GPIO_BUTTON_1 7
#define BTN_PINS_CNT 2
/* DIO13, LaunchPad Button BTN-1 (Left) */
extern const uint_least8_t CONFIG_GPIO_BUTTON_0_CONST;
/* DIO14, LaunchPad Button BTN-2 (Right) */
extern const uint_least8_t CONFIG_GPIO_BUTTON_1_CONST;
#else
#define BTN_PINS_CNT 0
#endif /*LAUNCHXL_CC26X2R1*/
#define GPIO_COUNT (2 +LORA_PINS_CNT+BTN_PINS_CNT)

/* DIO6, LaunchPad LED Red */
extern const uint_least8_t CONFIG_GPIO_LED_0_CONST;
/* DIO7, LaunchPad LED Green */
extern const uint_least8_t CONFIG_GPIO_LED_1_CONST;
#define CONFIG_TI_DRIVERS_GPIO_COUNT 4

/* LEDs are active high */
#define CONFIG_LED_ON (1)
#define CONFIG_LED_OFF (0)

extern PIN_Config BoardGpioInitTable[];

#ifdef LAUNCHXL_CC26X2R1
/* LaunchPad Button BTN-1 (Left), Parent Signal: CONFIG_GPIO_BUTTON_0 GPIO Pin, (DIO13) */
#define CONFIG_PIN_0 0x0000000d
/* LaunchPad Button BTN-2 (Right), Parent Signal: CONFIG_GPIO_BUTTON_1 GPIO Pin, (DIO14) */
#define CONFIG_PIN_1 0x0000000e
#define CONFIG_TI_DRIVERS_PIN_COUNT 4
#else
#define CONFIG_TI_DRIVERS_PIN_COUNT 2
#endif


bool gpio_init(void);

/*GPIO API*/
bool gpio_get_state(uint8_t io_pin, uint8_t* logic_level);
bool gpio_set_state(uint8_t io_pin, uint8_t logic_level);
bool is_edge_irq_en(uint8_t dio_pin);
bool gpio_toggle(uint8_t io_pin);
bool proc_led(void);
uint8_t get_mcu_pin(uint8_t io_pin);
uint8_t get_aux_num(uint8_t io_pin);
bool gpio_init_layout(const Pin_t* PinTable, uint8_t size);

#endif /* INIT_GPIO_H  */
