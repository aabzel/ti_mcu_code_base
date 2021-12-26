#ifndef FLASH_DRV_H
#define FLASH_DRV_H

#include <stdbool.h>
#include <stdint.h>

#include "macro_utils.h"

#define QWORD_LEN 4


/* Memory map for STM32F407 */
/* mm_page 1        - 128k : [40000 ... 60000)  - sectors 6 */
/* mm_page 2        - 128k : [60000 ... 80000)  - sectors 7 */
/* page 1 */
//#define MEMORY_MANAGER1_OFFSET  0x08040000

//#define MEMORY_MANAGER1_BANK    1
//#define MEMORY_MANAGER1_SECTOR  6
//#define MEMORY_MANAGER1_AMOUNT  1

/* page 2 */
//#define MEMORY_MANAGER2_OFFSET  0x08060000
//#define MEMORY_MANAGER2_BANK    1
//#define MEMORY_MANAGER2_SECTOR  7
//#define MEMORY_MANAGER2_AMOUNT  1


/*Hardware related code start*/
#define NOR_FLASH_BASE 0x08000000
#define NOR_FLASH_SIZE (1024 * K_BYTES)
#define NOR_FLASH_END (NOR_FLASH_BASE + NOR_FLASH_SIZE)
/*Hardware related code end*/
#define FLASH_WR_TIME_MS 20
#define FLASH_SECTOR_SIZE (128*K_BYTES)

bool flash_init(void);
bool flash_errase(uint32_t addr, uint32_t len);
bool flash_wr(uint32_t addr, uint8_t* array, uint32_t array_len);
bool flash_read(uint32_t in_flash_addr, uint8_t* rx_array, uint32_t array_len);
bool is_errased(uint32_t addr, uint32_t size);
bool is_flash_spare(uint32_t flash_addr, uint32_t size);
bool is_flash_addr(uint32_t flash_addr);
bool flash_find_spare_region(uint32_t* out_addr, uint32_t size);
bool flash_scan(uint8_t* base, uint32_t size, float* usage_pec, uint32_t* spare, uint32_t* busy);

#endif /* FLASH_DRV_H  */
