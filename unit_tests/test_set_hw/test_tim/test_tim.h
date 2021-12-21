#ifndef TEST_TIM_H
#define TEST_TIM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_tim_us(void) ;
bool test_tim_ms(void) ;

#define TEST_SUIT_TIM        \
    {"tim_us", test_tim_us}, \
    {"tim_ms", test_tim_ms},

#ifdef __cplusplus
}
#endif

#endif /* TEST_TIM_H */
