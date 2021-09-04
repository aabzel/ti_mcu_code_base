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
#include "flash_drv.h"
#include "flash_fs_ll.h"
#include "memory_layout.h"

/* status for a active page */
static const uint8_t mm_PageStateActive[QWORD_LEN] = {0x00, 0x00, 0xA5, 0x5A};

/* status for a ready page */
static const uint8_t mm_PageStateReady[QWORD_LEN] = {0x00, 0x00, 0xFF, 0xFF};

/* status for a clear page */
static const uint8_t mm_PageStateClear[QWORD_LEN] = {0xFF, 0xFF, 0xFF, 0xFF};

/**
 *     @brief    locate currently active page
 *
 *     @param    mm_page_start - pointer where active page start address will be stored
 *     @param    mm_page_len - pointer where active page length will be stored
 *     @retval    MM_RET code
 */
int mmiGetActivePage(uint32_t* mm_page_start, uint32_t* mm_page_len) {
    /* select active page */
    if(*(uint32_t*)MEMORY_MANAGER1_OFFSET == *(uint32_t*)mm_PageStateActive) {
        /* active page is #1 */
        *mm_page_start = MEMORY_MANAGER1_OFFSET;
        *mm_page_len = MEMORY_MANAGER1_LENGTH;
        return MM_RET_CODE_OK;
    } else if(*(uint32_t*)MEMORY_MANAGER2_OFFSET == *(uint32_t*)mm_PageStateActive) {
        /* active page is #2 */
        *mm_page_start = MEMORY_MANAGER2_OFFSET;
        *mm_page_len = MEMORY_MANAGER2_LENGTH;
        return MM_RET_CODE_OK;
    } else if(*(uint32_t*)MEMORY_MANAGER1_OFFSET == *(uint32_t*)mm_PageStateReady ||
              *(uint32_t*)MEMORY_MANAGER1_OFFSET == *(uint32_t*)mm_PageStateClear) {
        return MM_RET_CODE_SPARE;
    } else {
        return MM_RET_FATAL_ERROR;
    }
}

static uint32_t calcRemSize(uint32_t offset) {
    uint32_t rem_size = 0;
    uint32_t mm_page_start = 0;
    uint32_t cur_len = 0;
    uint32_t mm_page_len = 0;
    int ret = mmiGetActivePage(&mm_page_start, &mm_page_len);
    if(MM_RET_CODE_OK == ret) {
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
 *    @retval   MM_RET code
 */
static int mmiGetNext(uint32_t* offset, uint32_t maxOffset, uint32_t* completeLen) {
    mmItem_t* item = NULL;
    uint32_t rem_size = 0;
    uint32_t cur_offset = *offset;
    do {
        /* valid item could not be less that 5 bytes */
        if((maxOffset - ITEM_HEADER_SZ) < cur_offset) {
            return MM_RET_CODE_NO_MEMORY;
        }

        /* reference item */
        item = (mmItem_t*)cur_offset;

        /* if all header have 'ff's - this is a spare place */
        if((0xFFFF == item->length) && (0xFFFF == item->id) && (0xFF == item->crc8)) {
            /* empty space found */
            return MM_RET_NOT_FOUND;
        }
        rem_size = calcRemSize((uint32_t)cur_offset);
        /*is data with that length can be fitted in flash page?*/
        if(item->length < rem_size) {
            uint8_t calc_crc8 = crc8_autosar_calc((void*)(cur_offset + ITEM_HEADER_SZ), item->length);
            if(calc_crc8 != item->crc8) {
                cur_offset++;
                continue;
            } else {
                /*spot data file valid item */
                *offset = cur_offset;
                *completeLen = item->length + sizeof(mmItem_t);
            }
        } else {
            cur_offset++;
            continue;
        }

        return MM_RET_CODE_OK;

    } while(1);
}

/**
 *     @brief    search for a specified item
 *
 *     @param    mm_page_start - start address of the page
 *     @param    mm_page_len - number of bytes within page
 *     @param    file_id - id that should be found, MM_INVALID_FIELD if we try to find empty space
 *     @param    item - pointer to item that was located
 *     @param    empty - poitner to first empty item
 *     @param    remSpace - pointer where remaining space will be placed
 *     @retval    MM_RET code
 */
static int mmiFindField(uint16_t file_id, mmItem_t** item, mmItem_t** empty, uint32_t* remSpace) {
    int ret = -1;
    uint32_t completeLen = 0;
    uint32_t prev_item_offset = 0xFFFFFFFF;
    uint32_t mm_page_start = 0;
    uint32_t offset = 0;
    uint32_t mm_page_len = 0;
    bool res = false;
    /* select active page */
    ret = mmiGetActivePage(&mm_page_start, &mm_page_len);
    if(ret != MM_RET_CODE_OK) {
        return ret;
    }

    /* we should store previous variable and active one */
    prev_item_offset = 0xFFFFFFFF;
    offset = mm_page_start + QWORD_LEN;
    /* look through whole storage looking for specified item */
    while(MM_RET_CODE_OK == mmiGetNext(&offset, mm_page_start + mm_page_len, &completeLen)) {
        /* reference item */
        *item = (mmItem_t*)offset;

        /* if field's file_id match to search one, proceed it */
        if((*item)->id == file_id) {
            /* if something similar was already found, we should invalidate it first */
            if(0xFFFFFFFF != prev_item_offset) {
                /* to invalidate prev item just clear its id */
                res = mmiFlashZero(prev_item_offset, sizeof(mmItem_t));
                // TODO: invalidate data also
                if(false == res) {
                    return MM_RET_FATAL_ERROR;
                }
            }
            /* store found offset */
            prev_item_offset = offset;
        }

        /* shift offset and try again */
        offset += completeLen;
    }

    /* calculate remaining space */
    *remSpace = mm_page_len - (offset - mm_page_start);

    /* put pointer to first empty item */
    *empty = (mmItem_t*)offset;
    /* put pointer to located item */
    if(0xFFFFFFFF != prev_item_offset) {
        *item = (mmItem_t*)prev_item_offset;
        return MM_RET_CODE_OK;
    } else {
        *item = NULL;
        return MM_RET_NOT_FOUND;
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
uint32_t mm_getRemainingSpace(void) {
    mmItem_t *item = NULL, *empty = NULL;
    uint32_t remSpace = 0;

    /* get all info from private API */
    if(mmiFindField(MM_INVALID_FIELD, &item, &empty, &remSpace) != MM_RET_NOT_FOUND) {
        /* this is not a normal case - we have error or invalid field present, no memory available */
        return 0xFFFFFFFF;
    }

    /* return remaining free space */
    return remSpace;
}

/**
 *     @brief    switch used memory manager page and copy all valid data to the next one
 *
 *     @retval    MM_RET code
 */
int mm_turnThePage(void) {
    int ret = -1;
    uint32_t completeLen = 0;
    uint32_t mmPageActiveStart = 0, mmPageActiveLen = 0, mmPagePassiveStart = 0, offsetActive = 0, offsetPassive = 0;
    bool res = false;
    /* select active page */
    ret = mmiGetActivePage(&mmPageActiveStart, &mmPageActiveLen);
    if(ret != MM_RET_CODE_OK) {
        return ret;
    }

    /* erase passive page and get pointer to it */
    if(MEMORY_MANAGER1_OFFSET == mmPageActiveStart) {
        /* clear page # 2 */
        res = flash_erase(MEMORY_MANAGER2_OFFSET, MEMORY_MANAGER2_LENGTH);
        if(res) {
            ret = MM_RET_CODE_OK;
        }
        mmPagePassiveStart = MEMORY_MANAGER2_OFFSET;
    } else if(MEMORY_MANAGER2_OFFSET == mmPageActiveStart) {
        /* clear page # 1 */
        res = flash_erase(MEMORY_MANAGER1_OFFSET, MEMORY_MANAGER1_LENGTH);
        if(res) {
            ret = MM_RET_CODE_OK;
        }
        mmPagePassiveStart = MEMORY_MANAGER1_OFFSET;
    } else {
        return MM_RET_FATAL_ERROR;
    }
    if(ret != MM_RET_CODE_OK) {
        return ret;
    }

    /* we should store previous variable and active one */
    offsetActive = mmPageActiveStart + QWORD_LEN;
    offsetPassive = mmPagePassiveStart + QWORD_LEN;
    /* look through whole storage looking for specified item */
    while(mmiGetNext(&offsetActive, mmPageActiveStart + mmPageActiveLen, &completeLen) == MM_RET_CODE_OK) {
        /* copy all valid items to the passive page */
        res = mmiFlashWrite(offsetPassive, (uint8_t*)offsetActive, completeLen);
        if(false == res) {
            return MM_RET_FATAL_ERROR;
        }
        /* shift offsets and look for next item */
        offsetActive += completeLen;
        offsetPassive += completeLen;
    }

    /* indicate passive page as active */
    res = mmiFlashWrite(mmPagePassiveStart, (uint8_t*)mm_PageStateActive, sizeof(mm_PageStateActive));
    if(false == res) {
        return MM_RET_FATAL_ERROR;
    }
    /* indicate active page as passive */
    res = mmiFlashZero(mmPageActiveStart, 4);
    if(false == res) {
        return MM_RET_FATAL_ERROR;
    }
    /* invalidate active page */
    res = mmiFlashZero(offsetPassive, 4);
    if(false == res) {
        return MM_RET_FATAL_ERROR;
    }

    return MM_RET_CODE_OK;
}

/**
 *     @brief   Toggle Not Flash page in case of a lack of spare space
 *
 *     @note    call mm_maintain in spare time.
 *
 *     @retval MM_RET code
 */

int mm_maintain(void) {
    mmItem_t *prevItem = NULL, *empty = NULL;
    int ret = 0;
    uint32_t remSpace = 0;
    uint16_t data_id = 0; /*any number*/
    /* try to find previous file of the same variable */
    ret = mmiFindField(data_id, &prevItem, &empty, &remSpace);
    if(ret != MM_RET_CODE_OK && ret != MM_RET_NOT_FOUND) {
        return ret;
    }

    /* ensure that we have enough space within selected page to fit new file
     * variable item stores pointer to first empty item */
    if(remSpace < 4) {
        /* we have not enough space to fit new file, turn the page */
        ret = mm_turnThePage(); /*Long procedure*/
        if(ret != MM_RET_CODE_OK) {
            return ret;
        }

        /* refresh information about previous item, first free item and empty space */
        ret = mmiFindField(data_id, &prevItem, &empty, &remSpace);
        if(ret != MM_RET_CODE_OK && ret != MM_RET_NOT_FOUND) {
            return ret;
        }
    }
    return MM_RET_CODE_OK;
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
 *     @retval MM_RET code
 */
int mm_set(uint16_t data_id, uint8_t* new_file, uint16_t new_file_len) {
    mmItem_t *prevItem = NULL, *empty = NULL;
    mmItem_t newHeader = {0, 0, 0};
    int ret = 0;
    uint32_t remSpace = 0;
    uint32_t completeLen = 0;

    /* check input parameters */
    if((MM_INVALID_FIELD == data_id) || (NULL == new_file) || (0 == new_file_len)) {
        return MM_RET_BAD_PARAM;
    }

    /* try to find previous file of the same variable */
    ret = mmiFindField(data_id, &prevItem, &empty, &remSpace);
    if(ret != MM_RET_CODE_OK && ret != MM_RET_NOT_FOUND) {
        return ret;
    }
    if(MM_RET_CODE_OK == ret) {
        /* file was found - ensure that new file is not same to the previous one */
        if(prevItem->length == new_file_len) {
            if(0 == memcmp(new_file, (uint8_t*)prevItem + sizeof(mmItem_t), new_file_len)) {
                /* file is the same, we should not change anything */
                return MM_RET_CODE_OK;
            }
        }
    }

    /* calculate complete length of the new item */
    completeLen = new_file_len;
    /* ensure that we have enough space within selected page to fit new file
     * variable item stores pointer to first empty item */
    if(remSpace < completeLen) {
        /* we have not enough space to fit new file, turn the page */
#ifdef FLASH_FS_AUTO_TOGGLE_PAGE
        ret = mm_turnThePage();
        if(ret != MM_RET_CODE_OK) {
            return ret;
        }

        /* refresh information about previous item, first free item and empty space */
        ret = mmiFindField(data_id, &prevItem, &empty, &remSpace);
        if(ret != MM_RET_CODE_OK && ret != MM_RET_NOT_FOUND) {
            return ret;
        }

        /* ensure that now we have enough space */
        if(remSpace < completeLen) {
            return MM_RET_CODE_NO_MEMORY;
        }
#else
        return MM_RET_PAGE_FULL;
#endif /*AUTO_TOGGLE_PAGE*/
    }

    /* here we are sure that we should refresh file and have enough space to do that,
     * variable prevItem stores pointer to previous file of the item and item stores pointer to empty space */

    /* prepare header */
    newHeader.length = new_file_len;
    newHeader.id = data_id;
    newHeader.crc8 = crc8_autosar_calc(new_file, new_file_len);
    /* write Header*/
    if(mmiFlashWrite((uint32_t)empty, (uint8_t*)&newHeader, sizeof(newHeader)) == false) {
        return MM_RET_FATAL_ERROR;
    }

    /*  write full data blocks */
    if(false == mmiFlashWrite((uint32_t)empty + sizeof(mmItem_t), (uint8_t*)new_file, new_file_len)) {
        return MM_RET_FATAL_ERROR;
    }

    /* invalidate previous identifier (if present) */
    if(prevItem != NULL) {
        if(false == mmiFlashZero((uint32_t)prevItem, sizeof(mmItem_t))) {
            return MM_RET_FATAL_ERROR;
        }
    }

    return MM_RET_CODE_OK;
}

/**
 *    @brief    get address of the specified variable, stored within memory
 *
 *    @param    data_id - data identifier to be located
 *    @param    file_address - pointer where location of the variable will be written
 *    @param    fileLen - pointer to variable that will contain length of the variable
 *    @retval    MM_RET code
 */
int mm_getAddress(uint16_t data_id, uint8_t** file_address, uint16_t* fileLen) {
    mmItem_t *item, *empty;
    int ret;
    uint32_t remSpace;

    /* ensure that input parameters are valid */
    if(data_id == MM_INVALID_FIELD || file_address == NULL || fileLen == NULL) {
        return MM_RET_BAD_PARAM;
    }

    /* find field */
    ret = mmiFindField(data_id, &item, &empty, &remSpace);
    if(ret != MM_RET_CODE_OK) {
        return ret;
    }

    /* return parameters */
    *file_address = (uint8_t*)item + sizeof(mmItem_t);
    *fileLen = item->length;
    return MM_RET_CODE_OK;
}

/**
 *    @brief    get file of the specified variable
 *
 *    @param    data_id - data identifier to be read
 *    @param    file - pointer where data should be stored
 *    @param    maxValueLen - maximum number of bytes that could be stored within file
 *    @param    fileLen - actual number of bytes stored within file
 *    @retval MM_RET code
 */
int mm_get(uint16_t data_id, uint8_t* file, uint16_t maxValueLen, uint16_t* fileLen) {
    mmItem_t *item, *empty;
    int ret = 0;
    uint32_t remSpace = 0;

    /* ensure that input parameters are valid */
    if((MM_INVALID_FIELD == data_id) || (NULL == file) || (NULL == fileLen) || (0 == maxValueLen)) {
        return MM_RET_BAD_PARAM;
    }

    /* find field */
    ret = mmiFindField(data_id, &item, &empty, &remSpace);
    if(ret != MM_RET_CODE_OK) {
        return ret;
    }

    /* ensure that we have enough space in output buffer to hold new file */
    if(maxValueLen < item->length) {
        return MM_RET_CODE_NO_MEMORY;
    }

    /* copy data */
    memcpy(file, (uint8_t*)item + sizeof(mmItem_t), item->length);

    /* return parameters */
    *fileLen = item->length;
    return MM_RET_CODE_OK;
}

/**
 *     @brief    invalidate (erase) specified variable, all references to specified variable will be removed
 *
 *     @note    this function does not erase actual file of the variable
 *             if user need to do that (if file is very sensitive), double page swap MUST be executed
 *
 *     @param    data_id - data identifier to be invalidated
 *     @retval    MM_RET code
 */
int mm_invalidate(uint16_t data_id) {
    mmItem_t *item = NULL, *empty = NULL;
    int ret = 0;
    uint32_t remSpace = 0;
    bool res = false;
    /* find field */
    ret = mmiFindField(data_id, &item, &empty, &remSpace);
    if(ret != MM_RET_CODE_OK) {
        return ret;
    }

    /* invalidate field */
    res = mmiFlashZero((uint32_t)item, sizeof(mmItem_t));
    /*TODO invalidate data if crc match*/
    if(false == res) {
        return MM_RET_FATAL_ERROR;
    }

    return MM_RET_CODE_OK;
}
/**
 *    @brief    flash memory is not mapped yet, remap it
 *
 *    @retval    MM_RET code
 */
int mmiFlashFormat(void) {
    bool res = false;
    /* invalidate second page as passive */
    res = mmiFlashZero(MEMORY_MANAGER2_OFFSET, 4);
    if(false == res) {
        return MM_RET_FATAL_ERROR;
    }

    /* erase first page */
    res = flash_erase(MEMORY_MANAGER1_OFFSET, MEMORY_MANAGER1_LENGTH);
    if(false == res) {
        return MM_RET_FATAL_ERROR;
    }

    /* indicate first page as active */
    res = mmiFlashWrite(MEMORY_MANAGER1_OFFSET, (uint8_t*)mm_PageStateActive, sizeof(mm_PageStateActive));
    if(false == res) {
        return MM_RET_FATAL_ERROR;
    }

    return MM_RET_CODE_OK;
}
