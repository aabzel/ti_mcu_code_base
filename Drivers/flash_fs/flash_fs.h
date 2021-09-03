/**
 * @brief Definitions of Memory Manager methods
 *
 * This file contains definitions of Memory Manager  methods and constants
 *
 */

#ifndef MEMORY_MANAGER_NOR_FLASH_H
#define MEMORY_MANAGER_NOR_FLASH_H

#include <stdint.h>

#define QWORD_LEN 4
#define MIN_SIZE_OF_ITEM (sizeof(mmItem) + QWORD_LEN)
#define MM_INVALID_FIELD 0xFFFF

/* memory manager */
enum memManRetCodes {
    MM_RET_CODE_OK = 0,         /* OK */
    MM_RET_BAD_PARAM = 1,       /* Incorrect parameter */
    MM_RET_FATAL_ERROR = 4,     /* Fatal error */
    MM_RET_NOT_FOUND = 5,       /* Memory field isn't found */
    MM_RET_CODE_NO_MEMORY = 10, /* Isn't enough memory */
    MM_RET_CANNOT_OPEN = 11,
    MM_RET_CANNOT_GET = 12,
    MM_RET_LEN_ERROR = 13,
    MM_RET_NEW_VAL_MATCH_CUR_VAL = 14, /* now need to spoil flash*/
    MM_RET_CODE_SPARE = 15,            /* memory is not mapped yet*/
    MM_RET_PAGE_FULL = 16,             /* Page overflow */
    MM_RET_CNT
};

typedef struct xMMitem {
    uint16_t length;
    uint16_t nLength;
    uint16_t id;
    uint16_t nId;
#ifdef HAS_MM_CRC32
    uint32_t crc32;
#endif /*HAS_MM_CRC32*/
} mmItem;

int mm_maintain(void);
int mmiGetActivePage(uint32_t* mm_page_start, uint32_t* mm_page_len);
int mm_getRemainingSpace(void);
int mm_turnThePage(void);
int mm_set(uint16_t data_id, uint8_t* new_file, uint16_t new_file_len);
int mm_getAddress(uint16_t data_id, uint8_t** value_address, uint16_t* value_len);
int mm_get(uint16_t data_id, uint8_t* value, uint16_t max_value_len, uint16_t* value_len);
int mm_invalidate(uint16_t data_id);
int mmiFlashFormat(void);

#endif /* MEMORY_MANAGER_NOR_FLASH_H */
