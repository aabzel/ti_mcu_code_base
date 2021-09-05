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

#define QWORD_LEN 4
#define MM_INVALID_ID 0xFFFF

struct xMMitem_t {
    uint16_t id;
    uint16_t length;
    uint8_t crc8; /*only for payload*/
} __attribute__((packed));
typedef struct xMMitem_t mmItem_t;

#define ITEM_HEADER_SZ (sizeof(mmItem_t))

bool flash_fs_init(void);
bool mm_flash_format(void);
bool mm_get(uint16_t data_id, uint8_t* value, uint16_t max_value_len, uint16_t* value_len);
bool mm_get_active_page(uint32_t* mm_page_start, uint32_t* mm_page_len);
bool mm_get_address(uint16_t data_id, uint8_t** value_address, uint16_t* value_len);
bool mm_invalidate(uint16_t data_id);
bool mm_maintain(void);
bool mm_set(uint16_t data_id, uint8_t* new_file, uint16_t new_file_len);
bool mm_turn_page(void);
uint32_t mm_get_remaining_space(void);

#endif /* MEMORY_MANAGER_NOR_FLASH_H */
