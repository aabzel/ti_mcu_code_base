#include "read_mem.h"

#include <stddef.h>

//#include "rx_io.h"

uint8_t read_addr_8bit(uint32_t addr_val) {
  uint8_t val = 0u;
  if (0u != addr_val) {
    volatile uint8_t *addr = NULL;
    /*MISRA 2012 Rule 11.4: integer should not be converted to pointer */
    addr = (volatile uint8_t *)addr_val;
    val = *addr;
  }
  return val;
}

uint32_t read_addr_32bit(uint32_t addr_val) {
  uint32_t val = 0u;
  if (0u != addr_val) {
    volatile uint32_t *addr = NULL;
    /*MISRA 2012 Rule 11.4: integer should not be converted to pointer */
    addr = (volatile uint32_t *)addr_val;
    val = *addr;
  }

  return val;
}
