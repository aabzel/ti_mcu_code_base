/**
 * @brief Definitions of Memory Manager methods
 *
 * This file contains definitions of Memory Manager  methods and constants
 *
 */

#ifndef MEMORY_MANAGER_NOR_FLASH_H
#define MEMORY_MANAGER_NOR_FLASH_H

#include <stdbool.h>
#include <stdint.h>

#include "timer_utils.h"

#define QWORD_LEN 4
#define MM_INVALID_ID 0xFFFF
#define FLASH_FS_PERIOD_US SEC_2_US(1)

typedef struct xFlashFsPage_t {
    uint32_t files_cnt;
    uint8_t usage_pre_cent;
} FlashPageFs_t;

typedef struct xFlashFs_t {
    FlashPageFs_t page1;
    FlashPageFs_t page2;
} FlashFs_t;

struct xMMitem_t {
    uint16_t id;
    uint16_t nid; /* bit inverted id*/
    uint16_t length;
    uint8_t crc8;          /*only for payload*/
} __attribute__((packed)); /*to save flash memory*/
typedef struct xMMitem_t mmItem_t;

#define ITEM_HEADER_SZ (sizeof(mmItem_t))
#define MIN_FILE_LEN (ITEM_HEADER_SZ + 1)
//#define FLASH_FS_AUTO_TOGGLE_PAGE

extern FlashFs_t FlashFs;

bool flash_fs_init(void);
bool flash_fs_proc(void);
bool mm_flash_format(void);
bool mm_flash_erase(void);
bool mm_get(uint16_t data_id, uint8_t* value, uint16_t max_value_len, uint16_t* value_len);
bool mm_get_active_page(uint32_t* mm_page_start, uint32_t* mm_page_len);
bool mm_get_address(uint16_t data_id, uint8_t** value_address, uint16_t* value_len);
bool mm_invalidate(uint16_t data_id);
bool mm_maintain(void);
bool mm_set(uint16_t data_id, uint8_t* new_file, uint16_t new_file_len);
bool mm_turn_page(void);
uint32_t mm_cnt_files(uint32_t start_page_addr, uint32_t page_len, uint32_t* spare_cnt);
uint32_t mm_get_remaining_space(void);

#endif /* MEMORY_MANAGER_NOR_FLASH_H */
