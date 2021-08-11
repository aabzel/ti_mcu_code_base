#ifndef INIT_GPIO_H
#define INIT_GPIO_H

#include <stdbool.h>
#include <stdint.h>

#include <ti/drivers/PIN.h>

#define DIN31_0_REG_ADDR 0x400220C0
#define DOE31_0_REG_ADDR 0x400220D0
#define IOC_BASE 0x40081000

#define DIO_CNT 31

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

typedef struct xPin_t {
    uint8_t dio;
    uint8_t mcu_pin;
    uint8_t aux_pin;
} Pin_t;

extern const Pin_t PinTable[DIO_CNT];

bool gpio_init(void);

/*GPIO API*/
bool gpio_get_state(uint8_t io_pin, uint8_t* logic_level);
bool gpio_set_state(uint8_t io_pin, uint8_t logic_level);
const char* get_gpio_mode(uint8_t io_pin);
const char* get_gpio_type(uint8_t io_pin);
const char* get_pin_dir(uint8_t io_pin);
const char* get_gpio_alter_fun(uint8_t dio_pin);
const char* get_gpio_pull_mode(uint8_t dio_pin);
bool is_edge_irq_en(uint8_t dio_pin);
bool gpio_toggle(uint8_t io_pin);
bool proc_led(void);
const char* get_gpio_edge(uint8_t dio_pin);
uint8_t get_mcu_pin(uint8_t io_pin);
uint8_t get_aux_num(uint8_t io_pin);

#endif /* INIT_GPIO_H  */
