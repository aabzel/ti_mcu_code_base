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

#define CONFIG_TI_DRIVERS_GPIO_COUNT 4

typedef enum ePullMode_t { PULL_DOWN = 1, PULL_UP = 2, PULL_AIR = 3, PULL_UNDEF = 4 } PullMode_t;

bool gpio_init(void);
bool gpio_get_state(uint8_t io_pin, uint8_t* logic_level);
bool gpio_set_state(uint8_t io_pin, uint8_t logic_level);
bool is_edge_irq_en(uint8_t dio_pin);
bool gpio_toggle(uint8_t io_pin);
bool proc_led(void);
PullMode_t gpio_get_pull_mode(uint8_t dio_pin);
uint8_t get_mcu_pin(uint8_t io_pin);
uint8_t get_aux_num(uint8_t io_pin);
bool gpio_init_layout(const Pin_t* PinTable, uint8_t size);
GPIO_PinConfig gpio_get_cfg_dio(uint8_t dio_number);

#endif /* INIT_GPIO_H  */
