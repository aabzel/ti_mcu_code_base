#include "diag_sys.h"

#include <stdbool.h>
#include <stdint.h>

#include "io_utils.h"
#include "version.h"
#include "writer_generic.h"

bool find_addr_by_val(uint16_t byte_num, uint32_t val, uint32_t start_addr,
                      uint32_t end_addr) {
  bool res = false;
  uint32_t cnt = 0;
  if (1 == byte_num) {
    uint8_t value = (uint8_t)val;
    uint8_t *ptr = (uint8_t *)start_addr;
    for (ptr = (uint8_t *)start_addr; ptr < ((uint8_t *)end_addr); ptr++) {
      if ((*ptr) == value) {
        io_printf("%u: addr *(0x%p): 0x%x/%u" CRLF, cnt, ptr, value, value);
        res = true;
      }
    }
  }

  if (2 == byte_num) {
    uint16_t value = (uint16_t)val;
    uint16_t *ptr = (uint16_t *)start_addr;
    for (ptr = (uint16_t *)start_addr; ptr < ((uint16_t *)end_addr); ptr++) {
      if ((*ptr) == value) {
        io_printf("%u: addr *(0x%p): 0x%x/%u" CRLF, cnt, ptr, value, value);
        res = true;
      }
    }
  }

  if (4 == byte_num) {
    uint32_t value = (uint32_t)val;
    uint32_t *ptr = (uint32_t *)start_addr;
    for (ptr = (uint32_t *)start_addr; ptr < ((uint32_t *)end_addr); ptr++) {
      if ((*ptr) == value) {
        io_printf("%u: addr *(0x%p): 0x%x/%u" CRLF, cnt, ptr, value, value);
        res = true;
      }
    }
  }
  return res;
}

void print_vector_table(void) {
  io_printf(CRLF "Vector table" CRLF);
  io_printf("Stack: %x " CRLF, *((uint32_t *)0x00000000));
  io_printf("Reset: %x " CRLF, *((uint32_t *)0x00000004));
  io_printf("Clock Security System: %x " CRLF, *((uint32_t *)0x00000008));
  io_printf("All class of fault: %x " CRLF, *((uint32_t *)0x0000000C));
  io_printf("Memory management: %x " CRLF, *((uint32_t *)0x00000010));
  io_printf("Pre-fetch fault: %x " CRLF, *((uint32_t *)0x00000014));
  io_printf("Undefined instruction: %x " CRLF, *((uint32_t *)0x00000018));
  io_printf("USART1 global interrupt: %x " CRLF, *((uint32_t *)0x000000D4));
  io_printf("USART2 global interrupt: %x " CRLF, *((uint32_t *)0x000000D8));
}

extern void main(void);

void print_sysinfo(void) {
  io_printf("Reset handler: 0x%x " CRLF, *((uint32_t *)0x00000004));
  //io_printf("addr of SystemInit() 0x%p" CRLF, SystemInit);
  io_printf("addr of main() 0x%p" CRLF, main);

  io_printf("RAM: 320 KB" CRLF);
  io_printf("Flash: 1 MB" CRLF);
  io_printf("RAM addr:   0x%08x....0x%08x " CRLF, RAM_START, RAM_END);
  io_printf("Flash addr: 0x%08x....0x%08x " CRLF, ROM_START, ROM_END);
  io_printf("top-of-stack: %x " CRLF, *((uint32_t *)0x00000000));
  io_printf("boot memory start: %x " CRLF, *((uint32_t *)0x00000004));
  // io_printf("main %p", main);
  // io_printf("Stack size: %u " CRLF, _Min_Stack_Size);
  print_vector_table();
}

void Error_Handler(void) {
 // __disable_irq();
  while (1) {
  }
}

bool print_mem(uint8_t *addr, int32_t len, bool new_line) {
  io_printf( "0x");
  bool res = false;
  if (0 < len) {
    uint32_t pos ;
    for ( pos = 0; pos < len; pos++) {
      if (0 == (pos % 16)) {
        //io_printf(CRLF);
      }
      io_printf("%02x", *(addr + pos));

      res = true;
    }
    if (true == new_line) {
      io_printf(CRLF);
    }
  }
  return res;
}
