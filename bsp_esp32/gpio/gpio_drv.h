#ifndef GPIO_DRV_H
#define GPIO_DRV_H

#include <stdbool.h>
#include <stdint.h>

typedef enum ePullMode_t { PULL_DOWN = 1, PULL_UP = 2, PULL_AIR = 3, PULL_UNDEF = 4 } PullMode_t;

typedef enum eDioDirr_t {
    GPIO_DIR_IN = 0,
    GPIO_DIR_OUT = 1,
    GPIO_DIR_INOUT = 2,
    GPIO_DIR_NONE = 4,
    GPIO_DIR_UNDEF = 5
} DioDir_t;

DioDir_t gpio_get_dir(uint8_t dio_pin);

PullMode_t gpio_get_pull_mode(uint8_t dio_pin);
bool gpio_get_in_mode(uint8_t dio_pin);
bool gpio_get_state(uint8_t io_pin, uint8_t* logic_level);
bool gpio_init(void);
bool gpio_init_layout(const Pin_t* PinTable, uint8_t size);
bool gpio_set_dir(uint8_t dio_pin, DioDir_t des_dir);
const char* gpio_get_name(uint8_t io_pin);
bool gpio_set_in_mode(uint8_t dio_pin, bool is_in_mode);
bool gpio_set_pull_mode(uint8_t io_pin, PullMode_t pull_mode);
bool gpio_set_state(uint8_t io_pin, uint8_t logic_level);
bool gpio_toggle(uint8_t io_pin);
bool is_edge_irq_en(uint8_t dio_pin);
inline bool is_dio(uint8_t dio_pin);
uint32_t gpio_get_alter_fun(uint8_t dio_pin);
uint8_t gpio_read(uint8_t dio_number);
uint8_t get_aux_num(uint8_t io_pin);
uint8_t get_mcu_pin(uint8_t io_pin);
#endif /* GPIO_DRV_H  */
