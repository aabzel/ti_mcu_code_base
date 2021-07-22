#include <inttypes.h>
#include <stdint.h>

#include "device_id.h"
#include "io_utils.h"
#include "str_utils.h"
#include "version.h"

/*Unique device ID register (96 bits)*/
#define ID_UNIQUE_ADDRESS 0x1FFF7A10
#define DEVICE_ID_SIZE 12
#define ID_GET_UNIQUE_8(x) *(((const uint8_t *)ID_UNIQUE_ADDRESS) + x)
#define ID_GET_UNIQUE_32(x) *(((const uint32_t *)ID_UNIQUE_ADDRESS) + x)

void dump_device_id(void) {
  io_printf("0x%08" PRIX32 " 0x%08" PRIX32 " 0x%08" PRIX32 " ",
            ID_GET_UNIQUE_32(0), ID_GET_UNIQUE_32(1), ID_GET_UNIQUE_32(2));
}

uint64_t get_device_serial(void) {
  return ((uint64_t)ID_GET_UNIQUE_32(0)) ^
         ((uint64_t)ID_GET_UNIQUE_32(1)) << 7 ^
         ((uint64_t)ID_GET_UNIQUE_32(2)) << 16;
}
