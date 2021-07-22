#ifndef READ_MEM_H
#define READ_MEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

uint8_t read_addr_8bit(uint32_t addr_val);
uint32_t read_addr_32bit(uint32_t addr_val);

#ifdef __cplusplus
}
#endif

#endif /* READ_MEM_H */
