/**
 * NOR Flash Memory manager
 * This file contains implementation of key-val(file) map in Nor-Flash with
 * uniform deterioration of Nor Flash memory
 *
 */
#include "flash_fs.h"

#include <stdbool.h>
#include <string.h>

#include "crc8_autosar.h"
#include "data_utils.h"
#include "flash_fs_ll.h"
#include "flash_nvs_drv.h"
#include "io_utils.h"
#include "log.h"
#include "memory_layout.h"
#include "param_ids.h"

FlashFs_t FlashFs;

/* status for a active page */
static const uint8_t mm_PageStateActive[QWORD_LEN] = {0x00, 0x00, 0xa5, 0x5a};

/**
 *     @brief    locate currently active page
 *
 *     @param    mm_page_start - pointer where active page start address will be stored
 *     @param    mm_page_len - pointer where active page length will be stored
 *     @retval   exe status
 */
bool mm_get_active_page(uint32_t* mm_page_start, uint32_t* mm_page_len) {
    bool res = false;
    /* select active page */
    uint32_t page_state_active = 0;
    memcpy(&page_state_active, mm_PageStateActive, sizeof(mm_PageStateActive));
    uint32_t first_qword_page1 = 0;
    uint32_t first_qword_page2 = 0;
    memcpy((void*)&first_qword_page2, (void*)MEMORY_MANAGER2_OFFSET, QWORD_LEN);
    memcpy((void*)&first_qword_page1, (void*)MEMORY_MANAGER1_OFFSET, QWORD_LEN);
    if(first_qword_page1 == page_state_active) {
        /* active page is #1 */
        *mm_page_start = MEMORY_MANAGER1_OFFSET;
        *mm_page_len = MEMORY_MANAGER1_LENGTH;
        res = true;
    } else if(first_qword_page2 == page_state_active) {
        /* active page is #2 */
        *mm_page_start = MEMORY_MANAGER2_OFFSET;
        *mm_page_len = MEMORY_MANAGER2_LENGTH;
        res = true;
    } else {
        res = false;
    }
    return res;
}

static uint32_t calc_rem_size(uint32_t offset) {
    uint32_t rem_size = 0;
    uint32_t mm_page_start = 0;
    uint32_t cur_len = 0;
    uint32_t mm_page_len = 0;
    bool res = mm_get_active_page(&mm_page_start, &mm_page_len);
    if(true == res) {
        if(mm_page_start < offset) {
            cur_len = offset - mm_page_start;
            rem_size = mm_page_len - cur_len;
        } else {
            rem_size = 0;
        }
    }
    return rem_size;
}

/**
 *    @brief    finds next valid file item
 *
 *    @param    offset - pointer to store active offset in flash
 *    @param    maxOffset - maximum offset that could be in specified storage
 *    @param    len - pointer to store complete
 */
static bool mm_get_next(uint32_t* offset, uint32_t maxOffset, uint32_t* completeLen) {
    bool res = false;
    mmItem_t* item = NULL;
    uint32_t rem_size = 0;
    uint32_t cur_offset = *offset;
    /* valid item could not be less that MIN_FILE_LEN bytes */
    for(cur_offset = *offset; cur_offset < (maxOffset - MIN_FILE_LEN); cur_offset++) {
        /* reference item */
        item = (mmItem_t*)cur_offset;
        if(item->id != (MASK_16BIT ^ item->nid)) {
            continue;
        }
        /*spot id!*/
        rem_size = calc_rem_size((uint32_t)cur_offset);
        /*is data with that length can be fitted in flash page?*/
        if(item->length < rem_size) {
            uint8_t calc_crc8 = crc8_autosar_calc((void*)(cur_offset + ITEM_HEADER_SZ), item->length);
            if(calc_crc8 != item->crc8) {
                continue;
            } else {
                /*spot data file valid item */
                *offset = cur_offset;
                *completeLen = item->length + sizeof(mmItem_t);
            }
        } else {
            continue;
        }
        res = true;
        break;
    }
    return res;
}

/**
 * @brief count files in Flash File system
 *
 */
uint32_t mm_cnt_files(uint32_t start_page_addr, uint32_t page_len, uint32_t* spare_cnt) {
    mmItem_t* item = NULL;
    uint32_t rem_size = 0;
    bool res = false;
    uint32_t cur_offset = start_page_addr;
    uint32_t file_cnt = 0;
    uint8_t* byte_p = NULL;
    *spare_cnt = 0;
    for(cur_offset = start_page_addr; cur_offset <= (start_page_addr + page_len - MIN_FILE_LEN); cur_offset++) {
        /* reference item */
        byte_p = (uint8_t*)cur_offset;
        if(0xFF == *byte_p) {
            (*spare_cnt)++;
        }
        item = (mmItem_t*)cur_offset;

        if(item->id == (MASK_16BIT ^ item->nid)) {
            res = true;
        } else {
            res = false;
        }

        if(res) {
            rem_size = start_page_addr + page_len - cur_offset;
            /*is data with that length can be fitted in flash page?*/
            if((item->length < rem_size) && (0 < item->length)) {
                uint8_t calc_crc8 = crc8_autosar_calc((void*)(cur_offset + ITEM_HEADER_SZ), item->length);
                if(calc_crc8 == item->crc8) {
                    /*spot data file valid item */
                    file_cnt++;
                }
            }
        }
    }

    return file_cnt;
}

/**
 *     @brief    search for a specified item
 *
 *     @param    mm_page_start - start address of the page
 *     @param    mm_page_len - number of bytes within page
 *     @param    file_id - id that should be found, MM_INVALID_ID if we try to find empty space
 *     @param    item - pointer to item that was located
 *     @param    empty - poitner to first empty item
 *     @param    rem_space - pointer where remaining space will be placed
 *     @retval
 */
static bool mm_find_field(uint16_t file_id, mmItem_t** item, mmItem_t** empty, uint32_t* rem_space) {
    bool res = -1;
    uint32_t completeLen = 0;
    uint32_t prev_item_offset = 0xFFFFFFFF;
    mmItem_t* prevItem = NULL;
    uint32_t mm_page_start = 0;
    uint32_t offset = 0;
    uint32_t mm_page_len = 0;
    /* select active page */
    res = mm_get_active_page(&mm_page_start, &mm_page_len);
    if(false == res) {
        return false;
    }

    /* we should store previous variable and active one */
    prev_item_offset = 0xFFFFFFFF;
    offset = mm_page_start + QWORD_LEN;
    /* look through whole storage looking for specified item */
    while(true == mm_get_next(&offset, mm_page_start + mm_page_len, &completeLen)) {
        /* reference item */
        *item = (mmItem_t*)offset;

        /* if field's file_id match to search one, proceed it */
        if(((*item)->id == file_id) && ((*item)->id == (MASK_16BIT ^ (*item)->nid))) {
            /* if something similar was already found, we should invalidate it first */
            if(0xFFFFFFFF != prev_item_offset) {
                /* to invalidate prev item just clear its id */
                prevItem = (mmItem_t*)prev_item_offset;
                res = mm_flash_zero(prev_item_offset, sizeof(mmItem_t) + prevItem->length);
                if(false == res) {
                    return false;
                }
            }
            /* store found offset */
            prev_item_offset = offset;
        }

        /* shift offset and try again */
        offset += completeLen;
    }

    /* calculate remaining space */
    *rem_space = mm_page_len - (offset - mm_page_start);

    /* put pointer to first empty item */
    *empty = (mmItem_t*)offset;
    /* put pointer to located item */
    if(0xFFFFFFFF != prev_item_offset) {
        *item = (mmItem_t*)prev_item_offset;
        return true;
    } else {
        *item = NULL;
        return false;
    }
}

/**
 *     @brief    returns number of bytes remained within active page
 *
 *     This function is used to select moment of time when page swap (long operation) is most suitable
 *
 *     @return    number of bytes remained within active page
 *    @note    user must keep in mind that not all available memory could be used, memory manager use 8 bytes for
 *            every write attempt
 */
uint32_t mm_get_remaining_space(void) {
    mmItem_t *item = NULL, *empty = NULL;
    uint32_t rem_space = 0;

    /* get all info from private API */
    if(true == mm_find_field(MM_INVALID_ID, &item, &empty, &rem_space)) {
        /* this is not a normal case - we have error or invalid field present, no memory available */
        return 0xFFFFFFFF;
    }

    /* return remaining free space */
    return rem_space;
}

/**
 *    @brief    flash memory is not mapped yet, remap it
 */
bool mm_flash_format(void) {
    bool res = false;
    /* invalidate second page as passive */
    res = mm_flash_zero(MEMORY_MANAGER2_OFFSET, QWORD_LEN);
    if(res) {
        /* erase first page */
        res = flash_nvs_erase(MEMORY_MANAGER1_OFFSET, MEMORY_MANAGER1_LENGTH);
    }

    if(res) {
        /* indicate first page as active */
        res = mm_flash_write(MEMORY_MANAGER1_OFFSET, (uint8_t*)mm_PageStateActive, sizeof(mm_PageStateActive));
    }

    return res;
}

/**
 *     @brief    switch used memory manager page and copy all valid data to the next one
 *
 *     @retval
 */
bool mm_turn_page(void) {
    bool res = false;
    uint32_t completeLen = 0;
    uint32_t mmPageActiveStart = 0, mmPageActiveLen = 0;
    uint32_t mmPagePassiveStart = 0;
    uint32_t offsetActive = 0, offsetPassive = 0;
    /* select active page */
    res = mm_get_active_page(&mmPageActiveStart, &mmPageActiveLen);
    if(false == res) {
        return false;
    }

    /* erase passive page and get pointer to it */
    if(MEMORY_MANAGER1_OFFSET == mmPageActiveStart) {
        /* clear page # 2 */
        res = flash_nvs_erase(MEMORY_MANAGER2_OFFSET, MEMORY_MANAGER2_LENGTH);
        if(false == res) {
            return false;
        }
        mmPagePassiveStart = MEMORY_MANAGER2_OFFSET;
    } else if(MEMORY_MANAGER2_OFFSET == mmPageActiveStart) {
        /* clear page # 1 */
        res = flash_nvs_erase(MEMORY_MANAGER1_OFFSET, MEMORY_MANAGER1_LENGTH);
        if(false == res) {
            return false;
        }
        mmPagePassiveStart = MEMORY_MANAGER1_OFFSET;
    } else {
        return false;
    }
    if(false == res) {
        return res;
    }

    /* we should store previous variable and active one */
    offsetActive = mmPageActiveStart + QWORD_LEN;
    offsetPassive = mmPagePassiveStart + QWORD_LEN;
    /* look through whole storage looking for specified item */
    while(true == mm_get_next(&offsetActive, mmPageActiveStart + mmPageActiveLen, &completeLen)) {
        /* copy all valid items to the passive page */
        res = mm_flash_write(offsetPassive, (uint8_t*)offsetActive, completeLen);
        if(false == res) {
            return false;
        }
        /* shift offsets and look for next item */
        offsetActive += completeLen;
        offsetPassive += completeLen;
    }

    /* indicate passive page as active */
    res = mm_flash_write(mmPagePassiveStart, (uint8_t*)mm_PageStateActive, sizeof(mm_PageStateActive));
    if(false == res) {
        return false;
    }
    /* indicate active page as passive */
    res = mm_flash_zero(mmPageActiveStart, QWORD_LEN);
    if(false == res) {
        return false;
    }
    return true;
}

/**
 *     @brief   Toggle Not Flash page in case of a lack of spare space
 *
 *     @note    call mm_maintain in spare time.
 *
 *     @retval
 */

bool mm_maintain(void) {
    mmItem_t *prevItem = NULL, *empty = NULL;
    bool res = false;
    uint32_t rem_space = 100 * MIN_FILE_LEN;
    /* try to find previous file of the same variable */
    res = mm_find_field(MM_INVALID_ID, &prevItem, &empty, &rem_space);
    if(false == res) {
        res = true;
        if(rem_space < MIN_FILE_LEN) {
            /* we have not enough space to fit minimal payload file, turn the page */
            LOG_INFO(FLASH_FS, "Toggle NVS pages");
            res = mm_turn_page(); /*Long procedure*/
        }
    }
    return res;
}

bool flash_fs_proc(void) {
    bool res = false;
    res = mm_maintain();
    uint32_t spare_cnt = 0;
    spare_cnt = 0;
    FlashFs.page1.files_cnt = mm_cnt_files(MEMORY_MANAGER1_OFFSET, MEMORY_MANAGER1_LENGTH, &spare_cnt);
    FlashFs.page1.usage_pre_cent = 100 - ((100U * spare_cnt) / MEMORY_MANAGER1_LENGTH);

    spare_cnt = 0;
    FlashFs.page2.files_cnt = mm_cnt_files(MEMORY_MANAGER2_OFFSET, MEMORY_MANAGER2_LENGTH, &spare_cnt);
    FlashFs.page2.usage_pre_cent = 100 - ((100U * spare_cnt) / MEMORY_MANAGER2_LENGTH);

    return res;
}

/**
 *     @brief   set new file to the variable with specified ID
 *
 *     @note    operation from user point of view is atomic - if something goes wrong during memory write,
 *             old file will be stored in memory, if old file doesn't exist - new variable will not be created
 *
 *     @param    data_id - data identifier to be written
 *             user could operate with data_id '0000' - '7FFF', other are reserved to the system
 *     @param    new_file - pointer to the new file of the variable
 *     @param    new_file_len - number of bytes within new_file
 *     @retval
 */
bool mm_set(uint16_t data_id, uint8_t* new_file, uint16_t new_file_len) {
    mmItem_t *prevItem = NULL, *empty = NULL;
    mmItem_t newHeader = {0, 0, 0};
    bool res = 0;
    uint32_t rem_space = 0;
    uint32_t completeLen = 0;

    /* check input parameters */
    if((MM_INVALID_ID == data_id) || (NULL == new_file) || (0 == new_file_len)) {
        return false;
    }

    /* try to find previous file of the same variable */
    res = mm_find_field(data_id, &prevItem, &empty, &rem_space);
    if(true == res) {
        /* file was found - ensure that new file is not same to the previous one */
        if(prevItem->length == new_file_len) {
            if(0 == memcmp(new_file, (uint8_t*)prevItem + sizeof(mmItem_t), new_file_len)) {
                /* file is the same, we should not change anything */
                return true;
            }
        }
    }

    /* calculate complete length of the new item */
    completeLen = new_file_len;
    /* ensure that we have enough space within selected page to fit new file
     * variable item stores pointer to first empty item */
    if(rem_space < completeLen) {
        /* we have not enough space to fit new file, turn the page */
#ifdef FLASH_FS_AUTO_TOGGLE_PAGE
        res = mm_turn_page();
        if(false == res) {
            return false;
        }

        /* refresh information about previous item, first free item and empty space */
        res = mm_find_field(data_id, &prevItem, &empty, &rem_space);
        if(false == res) {
            return false;
        }

        /* ensure that now we have enough space */
        if(rem_space < completeLen) {
            return false;
        }
#else
        return false;
#endif /*AUTO_TOGGLE_PAGE*/
    }

    /* here we are sure that we should refresh file and have enough space to do that,
     * variable prevItem stores pointer to previous file of the item and item stores pointer to empty space */

    /* prepare header */
    newHeader.id = data_id;
    newHeader.nid = MASK_16BIT ^ data_id;
    newHeader.length = new_file_len;
    newHeader.crc8 = crc8_autosar_calc(new_file, new_file_len);
    /* write Header*/
    if(mm_flash_write((uint32_t)empty, (uint8_t*)&newHeader, sizeof(newHeader)) == false) {
        return false;
    }

    /*  write full data blocks */
    if(false == mm_flash_write((uint32_t)empty + sizeof(mmItem_t), (uint8_t*)new_file, new_file_len)) {
        return false;
    }

    /* invalidate previous identifier (if present) */
    if(prevItem != NULL) {
        if(false == mm_flash_zero((uint32_t)prevItem, sizeof(mmItem_t))) {
            return false;
        }
    }

    return true;
}

/**
 *    @brief    get address of the specified variable, stored within memory
 *
 *    @param    data_id - data identifier to be located
 *    @param    file_address - pointer where location of the variable will be written
 *    @param    fileLen - pointer to variable that will contain length of the variable

 */
bool mm_get_address(uint16_t data_id, uint8_t** file_address, uint16_t* fileLen) {
    mmItem_t *item = NULL, *empty = NULL;
    bool res = false;
    uint32_t rem_space = 0;

    /* ensure that input parameters are valid */
    if(data_id == MM_INVALID_ID || file_address == NULL || fileLen == NULL) {
        res = false;
    } else {
        res = true;
    }

    if(res) {
        /* find field */
        res = mm_find_field(data_id, &item, &empty, &rem_space);
    }

    if(res) {
        /* return parameters */
        *file_address = (uint8_t*)item + sizeof(mmItem_t);
        *fileLen = item->length;
    }
    return res;
}

/**
 *    @brief    get file of the specified variable
 *
 *    @param    data_id - data identifier to be read
 *    @param    file - pointer where data should be stored
 *    @param    maxValueLen - maximum number of bytes that could be stored within file
 *    @param    fileLen - actual number of bytes stored within file

 */
bool mm_get(uint16_t data_id, uint8_t* file, uint16_t maxValueLen, uint16_t* fileLen) {
    mmItem_t *item = NULL, *empty = NULL;
    bool res = false;
    uint32_t rem_space = 0;

    /* ensure that input parameters are valid */
    if((MM_INVALID_ID == data_id) || (NULL == file) || (NULL == fileLen) || (0 == maxValueLen)) {
        res = false;
    } else {
        res = true;
    }

    if(res) {
        /* find field */
        res = mm_find_field(data_id, &item, &empty, &rem_space);
    }

    if(res) {
        /* ensure that we have enough space in output buffer to hold new file */
        if(maxValueLen < item->length) {
            res = false;
        }
    }
    if(res) {
        /* copy data */
        memcpy(file, (uint8_t*)item + sizeof(mmItem_t), item->length);
        /* return parameters */
        *fileLen = item->length;
    }
    return res;
}

/**
 *     @brief    invalidate (erase) specified variable, all references to specified variable will be removed
 *
 *     @note    this function does not erase actual file of the variable
 *             if user need to do that (if file is very sensitive), double page swap MUST be executed
 *
 *     @param    data_id - data identifier to be invalidated
 *     @retval
 */
bool mm_invalidate(uint16_t data_id) {
    bool res = false;
    mmItem_t *item = NULL, *empty = NULL;
    uint32_t rem_space = 0;
    /* find field */
    res = mm_find_field(data_id, &item, &empty, &rem_space);

    if(res) {
        /* invalidate field */
        res = mm_flash_zero((uint32_t)item, sizeof(mmItem_t) + item->length);
    }

    return res;
}

/**
 *    @brief    flash memory is not mapped yet, remap it
 */
bool mm_flash_erase(void) {
    bool res = true;
    /* invalidate second page as passive */
    res = flash_nvs_erase(MEMORY_MANAGER1_OFFSET, MEMORY_MANAGER1_LENGTH) && res;
    res = flash_nvs_erase(MEMORY_MANAGER2_OFFSET, MEMORY_MANAGER2_LENGTH) && res;
    return res;
}

bool flash_fs_init(void) {
    bool res = false;
    uint32_t mm_page_start = 0;
    uint32_t mm_page_len = 0;
    res = mm_get_active_page(&mm_page_start, &mm_page_len);
    if(false == res) {
        LOG_WARNING(FLASH_FS, "format Flash FS");
        res = mm_flash_format();
    } else {
        LOG_INFO(FLASH_FS, "start: 0x%08x len %u", mm_page_start, mm_page_len);
    }
    uint16_t reboot_cnt = 0;
    uint16_t fileLen = 0;
    res = mm_get(PAR_ID_REBOOT_CNT, (uint8_t*)&reboot_cnt, sizeof(reboot_cnt), &fileLen);
    if(res) {
        reboot_cnt++;
        LOG_INFO(FLASH_FS, "reboot cnt: %u", reboot_cnt);
        res = mm_set(PAR_ID_REBOOT_CNT, (uint8_t*)&reboot_cnt, sizeof(reboot_cnt));
    } else {
        LOG_WARNING(FLASH_FS, "lack of reboot counter");
        reboot_cnt = 0;
        res = mm_set(PAR_ID_REBOOT_CNT, (uint8_t*)&reboot_cnt, sizeof(reboot_cnt));
        if(false == res) {
            LOG_WARNING(FLASH_FS, "Reset reboot counter");
        }
    }
    return res;
}

uint8_t addr2page_num(uint32_t mm_page_start) {
    uint8_t page_num = 0xFF;
    switch(mm_page_start) {
    case MEMORY_MANAGER1_OFFSET:
        page_num = 1;
        break;
    case MEMORY_MANAGER2_OFFSET:
        page_num = 2;
        break;
    default:
        page_num = 0x55;
        break;
    }
    return page_num;
}
