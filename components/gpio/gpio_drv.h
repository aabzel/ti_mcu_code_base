#ifndef INIT_GPIO_H
#define INIT_GPIO_H

#include <stdbool.h>
#include <stdint.h>

#include <ti/drivers/PIN.h>

#define CONFIG_GPIO_COUNT 4
/* DIO13, LaunchPad Button BTN-1 (Left) */
extern const uint_least8_t CONFIG_GPIO_BUTTON_0_CONST;
#define CONFIG_GPIO_BUTTON_0 0
/* DIO14, LaunchPad Button BTN-2 (Right) */
extern const uint_least8_t CONFIG_GPIO_BUTTON_1_CONST;
#define CONFIG_GPIO_BUTTON_1 1
/* DIO6, LaunchPad LED Red */
extern const uint_least8_t CONFIG_GPIO_LED_0_CONST;
#define CONFIG_GPIO_LED_0 2
/* DIO7, LaunchPad LED Green */
extern const uint_least8_t CONFIG_GPIO_LED_1_CONST;
#define CONFIG_GPIO_LED_1 3
#define CONFIG_TI_DRIVERS_GPIO_COUNT 4

/* LEDs are active high */
#define CONFIG_GPIO_LED_ON (1)
#define CONFIG_GPIO_LED_OFF (0)

#define CONFIG_LED_ON (CONFIG_GPIO_LED_ON)
#define CONFIG_LED_OFF (CONFIG_GPIO_LED_OFF)

extern const PIN_Config BoardGpioInitTable[];

/* LaunchPad Button BTN-1 (Left), Parent Signal: CONFIG_GPIO_BUTTON_0 GPIO Pin, (DIO13) */
#define CONFIG_PIN_0 0x0000000d
/* LaunchPad Button BTN-2 (Right), Parent Signal: CONFIG_GPIO_BUTTON_1 GPIO Pin, (DIO14) */
#define CONFIG_PIN_1 0x0000000e
/* LaunchPad LED Red, Parent Signal: CONFIG_GPIO_LED_0 GPIO Pin, (DIO6) */
#define CONFIG_PIN_2 0x00000006
/* LaunchPad LED Green, Parent Signal: CONFIG_GPIO_LED_1 GPIO Pin, (DIO7) */
#define CONFIG_PIN_3 0x00000007
#define CONFIG_TI_DRIVERS_PIN_COUNT 4

void gpio_init(void);

/*GPIO API*/
bool gpio_get_state(char port_pin_char, uint8_t port_pin_num, uint8_t *logic_level);
bool gpio_set_state(char port_pin_char, uint8_t port_pin_num, uint8_t logic_level);
char *get_gpio_mode(char port, uint8_t pin);
uint8_t get_gpio_alter_fun(char port, uint8_t pin);
char* get_gpio_pull_mode(char port, uint8_t pin);
char* get_gpio_type(char port, uint8_t pin);
uint8_t get_mcu_pin(char port, uint8_t pin);
bool gpio_toggle(char port, uint8_t pin);

#endif /* INIT_GPIO_H  */
