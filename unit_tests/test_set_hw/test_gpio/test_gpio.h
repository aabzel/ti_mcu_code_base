#ifndef TEST_GPIO_H
#define TEST_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_gpio_write(void) ;
//bool test_gpio_read(void) ;

#define TEST_SUIT_GPIO              \
    {"gpio_write", test_gpio_write},


//{"gpio_read", test_gpio_read},

#ifdef __cplusplus
}
#endif

#endif /* TEST_GPIO_H */
