#ifndef DIAG_SYS_H
#define DIAG_SYS_H

#ifdef __cplusplus
extern "C" {
#endif

//#include "stm32f4xx.h"

#include <stdbool.h>
#include <stdint.h>

#include "sys.h"

/*TODO: cacl frequency from system registers*/

#define RAM_SIZE (80 * 1024)
#define RAM_START 0x20000000
#define RAM_END (RAM_START + RAM_SIZE)

#define ROM_SIZE ( 352 * 1024)
#define ROM_START 0x00000000
#define ROM_END (ROM_START + ROM_SIZE)

void print_vector_table(void);
void print_sysinfo(void);
void Error_Handler(void);
bool find_addr_by_val(uint16_t byte_num, uint32_t val, uint32_t start_addr,
                      uint32_t end_addr);
bool print_mem(uint8_t *addr, int32_t len, bool new_line);

#ifdef __cplusplus
}
#endif

#endif /* DIAG_SYS_H */
