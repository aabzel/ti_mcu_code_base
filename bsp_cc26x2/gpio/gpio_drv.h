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

//#define DIN31_0_REG_ADDR (GPIO_BASE+GPIO_O_DIN31_0)
//#define DOE31_0_REG_ADDR (GPIO_BASE+GPIO_O_DOE31_0)

#define CONFIG_GPIO_COUNT 8

#define CONFIG_GPIO_BUTTON_0 0
#define CONFIG_GPIO_BUTTON_1 1
#define CONFIG_GPIO_LED_0 2
#define CONFIG_GPIO_LED_1 3
#define CONFIG_GPIO_LORA_CS 4
#define CONFIG_GPIO_LORA_RST 5
#define CONFIG_GPIO_LORA_INT 6
#define CONFIG_GPIO_LORA_BSY 7

/* DIO13, LaunchPad Button BTN-1 (Left) */
extern const uint_least8_t CONFIG_GPIO_BUTTON_0_CONST;
/* DIO14, LaunchPad Button BTN-2 (Right) */
extern const uint_least8_t CONFIG_GPIO_BUTTON_1_CONST;

/* DIO6, LaunchPad LED Red */
extern const uint_least8_t CONFIG_GPIO_LED_0_CONST;
/* DIO7, LaunchPad LED Green */
extern const uint_least8_t CONFIG_GPIO_LED_1_CONST;
#define CONFIG_TI_DRIVERS_GPIO_COUNT 4

/* LEDs are active high */
#define CONFIG_GPIO_LED_ON (1)
#define CONFIG_GPIO_LED_OFF (0)

#define CONFIG_LED_ON (CONFIG_GPIO_LED_ON)
#define CONFIG_LED_OFF (CONFIG_GPIO_LED_OFF)

extern PIN_Config BoardGpioInitTable[];

/* LaunchPad Button BTN-1 (Left), Parent Signal: CONFIG_GPIO_BUTTON_0 GPIO Pin, (DIO13) */
#define CONFIG_PIN_0 0x0000000d
/* LaunchPad Button BTN-2 (Right), Parent Signal: CONFIG_GPIO_BUTTON_1 GPIO Pin, (DIO14) */
#define CONFIG_PIN_1 0x0000000e
/* LaunchPad LED Red, Parent Signal: CONFIG_GPIO_LED_0 GPIO Pin, (DIO6) */
#define CONFIG_PIN_2 0x00000006
/* LaunchPad LED Green, Parent Signal: CONFIG_GPIO_LED_1 GPIO Pin, (DIO7) */
#define CONFIG_PIN_3 0x00000007
#define CONFIG_TI_DRIVERS_PIN_COUNT 4

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
