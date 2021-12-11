#ifndef CORE_DRIVER_H
#define CORE_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

bool is_ram_addr(uint32_t address);
bool try_recursion(uint32_t max_depth, uint32_t* stack_size);
float stack_used(void);

#ifdef __cplusplus
}
#endif

#endif /* CORE_DRIVER_H */
