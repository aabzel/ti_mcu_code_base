/**
 * NOR Flash Memory manager
 * This file contains implementation of key-val(file) map in Nor-Flash with
 * uniform deterioration of Nor Flash memory
 *
 */
#include "flash_fs.h"

#include <stdbool.h>
#include <string.h>

#include "flash_drv.h"
#include "flash_fs_ll.h"
#include "memory_layout.h"

/* status for a passive page */
static const uint8_t mm_PageStatePassive[QWORD_LEN] = {0x00, 0x00, 0x00, 0x00};

/* status for a active page */
static const uint8_t mm_PageStateActive[QWORD_LEN] = {0x00, 0x00, 0xA5, 0x5A};

/* status for a ready page */
static const uint8_t mm_PageStateReady[QWORD_LEN] = {0x00, 0x00, 0xFF, 0xFF};

/* status for a clear page */
static const uint8_t mm_PageStateClear[QWORD_LEN] = {0xFF, 0xFF, 0xFF, 0xFF};

/* removed id */
static const uint8_t mm_EraseMask[QWORD_LEN] = {0x00, 0x00, 0x00, 0x00};

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

/**
 *    @brief    finds next valid file item
 *
 *    @param    offset - pointer to store active offset
 *    @param    maxOffset - maximum offset that could be in specified storage
 *    @param    len - pointer to store complete
 *    @retval   MM_RET code
 */
static int mmiGetNext(uint32_t* offset, uint32_t maxOffset, int* completeLen) {
    mmItem* item = NULL;

    do {
        /* valid item could not be less that 12 bytes */
        if((maxOffset - MIN_SIZE_OF_ITEM) < (*offset)) {
            return MM_RET_CODE_NO_MEMORY;
        }

        /* reference item */
        item = (mmItem*)(*offset);

        /* if all header have 'ff's - this is a spare place */
        if(item->length == 0xFFFF && item->nLength == 0xFFFF && item->id == 0xFFFF && item->nId == 0xFFFF) {
            /* empty space found */
            return MM_RET_NOT_FOUND;
        }

        /* if length doesn't match to it's inversion, it is corrupted field and we should goto next one */
        if(item->length != (item->nLength ^ 0xFFFF)) {
            *offset += QWORD_LEN;
            continue;
        }

        /* fields are aligned to 4 bytes and have 8 bytes header */
        *completeLen = item->length + sizeof(mmItem);
        if((item->length & 0x03) != 0) {
            *completeLen += QWORD_LEN;
            *completeLen &= 0xFFFC;
        }

        /* if id field doesn't match to it's inversion - this is invalid field and we should skip it */
        if(item->id != (item->nId ^ 0xFFFF)) {
            *offset += *completeLen;
            continue;
        }

        /* if all checks was passed, we have found valid item */
        return MM_RET_CODE_OK;

    } while(1);
}

/**
 *     @brief    search for a specified item
 *
 *     @param    mm_page_start - start address of the page
 *     @param    mm_page_len - number of bytes within page
 *     @param    id - id that should be found, MM_INVALID_FIELD if we try to find empty space
 *     @param    item - pointer to item that was located
 *     @param    empty - poitner to first empty item
 *     @param    remSpace - pointer where remaining space will be placed
 *     @retval    MM_RET code
 */
static int mmiFindField(uint16_t file_id, mmItem** item, mmItem** empty, int* remSpace) {
    int ret = -1, completeLen = 0, itemOffset = 0;
    uint32_t mm_page_start = 0, mm_page_len = 0, offset = 0;
    bool res = false;
    /* select active page */
    ret = mmiGetActivePage(&mm_page_start, &mm_page_len);
    if(ret != MM_RET_CODE_OK) {
        return ret;
    }

    /* we should store previous variable and active one */
    itemOffset = -1;
    offset = mm_page_start + QWORD_LEN;
    /* look through whole storage looking for specified item */
    while(MM_RET_CODE_OK == mmiGetNext(&offset, mm_page_start + mm_page_len, &completeLen)) {
        /* reference item */
        *item = (mmItem*)offset;

        /* if field's file_id match to search one, proceed it */
        if((*item)->id == file_id) {
            /* if something similar was already found, we should invalidate it first */
            if(itemOffset != -1) {
                /* to invalidate item just clear its id */
                res = mmiFlashWrite(itemOffset + QWORD_LEN, (uint8_t*)mm_EraseMask, QWORD_LEN);
                if(false == res) {
                    return MM_RET_FATAL_ERROR;
                }
            }
            /* store found offset */
            itemOffset = offset;
        }

        /* shift offset and try again */
        offset += completeLen;
    }

    /* calculate remaining space */
    *remSpace = mm_page_len - (offset - mm_page_start);

    /* put pointer to first empty item */
    *empty = (mmItem*)offset;
    /* put pointer to located item */
    if(itemOffset != -1) {
        *item = (mmItem*)itemOffset;
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
int mm_getRemainingSpace(void) {
    mmItem *item = NULL, *empty = NULL;
    int remSpace = 0;

    /* get all info from private API */
    if(mmiFindField(MM_INVALID_FIELD, &item, &empty, &remSpace) != MM_RET_NOT_FOUND) {
        /* this is not a normal case - we have error or invalid field present, no memory available */
        return -1;
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
    int ret = -1, completeLen = 0;
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
    res = mmiFlashWrite(mmPageActiveStart, (uint8_t*)mm_PageStatePassive, sizeof(mm_PageStatePassive));
    if(false == res) {
        return MM_RET_FATAL_ERROR;
    }
    /* invalidate active page */
    res = mmiFlashWrite(offsetPassive, (uint8_t*)mm_PageStatePassive, sizeof(mm_PageStatePassive));
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
    mmItem *prevItem = NULL, *empty = NULL;
    int ret, remSpace = 0;
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
    mmItem *prevItem = NULL, *empty = NULL;
    mmItem newHeader = {0, 0, 0, 0};
    uint8_t tBuf[QWORD_LEN];
    int ret, remSpace, completeLen, tmp;

    /* check input parameters */
    if(data_id == MM_INVALID_FIELD || new_file == NULL || new_file_len == 0) {
        return MM_RET_BAD_PARAM;
    }

    /* try to find previous file of the same variable */
    ret = mmiFindField(data_id, &prevItem, &empty, &remSpace);
    if(ret != MM_RET_CODE_OK && ret != MM_RET_NOT_FOUND) {
        return ret;
    }
    if(ret == MM_RET_CODE_OK) {
        /* file was found - ensure that new file is not same to the previous one */
        if(prevItem->length == new_file_len) {
            if(memcmp(new_file, (uint8_t*)prevItem + sizeof(mmItem), new_file_len) == 0) {
                /* file is the same, we should not change anything */
                return MM_RET_CODE_OK;
            }
        }
    }

    /* calculate complete length of the new item */
    completeLen = new_file_len;
    if((new_file_len & 0x03) != 0) {
        completeLen += QWORD_LEN;
        completeLen &= 0xFFFC;
    }
    completeLen += 8;

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
    newHeader.nLength = new_file_len ^ 0xFFFF;
    newHeader.id = data_id;
    newHeader.nId = data_id ^ 0xFFFF;

    /* write Header*/
    if(mmiFlashWrite((uint32_t)empty, (uint8_t*)&newHeader, sizeof(newHeader)) != true) {
        return MM_RET_FATAL_ERROR;
    }

    /* then write full data blocks */
    for(tmp = 0; tmp < (new_file_len - 3); tmp += QWORD_LEN) {
        if(mmiFlashWrite((uint32_t)empty + sizeof(mmItem) + tmp, (uint8_t*)(new_file + tmp), QWORD_LEN) != true) {
            return MM_RET_FATAL_ERROR;
        }
    }

    /* after that write last block (if present) */
    if(tmp < new_file_len) {
        memcpy(tBuf, new_file + tmp, (new_file_len - tmp));
        if(mmiFlashWrite((uint32_t)empty + sizeof(mmItem) + tmp, (uint8_t*)tBuf, QWORD_LEN) != true) {
            return MM_RET_FATAL_ERROR;
        }
    }

    /* invalidate previous identifier (if present) */
    if(prevItem != NULL) {
        if(mmiFlashWrite((uint32_t)prevItem + QWORD_LEN, (uint8_t*)mm_EraseMask, QWORD_LEN) != true) {
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
    mmItem *item, *empty;
    int ret, remSpace;

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
    *file_address = (uint8_t*)item + sizeof(mmItem);
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
    mmItem *item, *empty;
    int ret, remSpace;

    /* ensure that input parameters are valid */
    if(data_id == MM_INVALID_FIELD || file == NULL || fileLen == NULL || maxValueLen == 0) {
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
    memcpy(file, (uint8_t*)item + sizeof(mmItem), item->length);

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
    mmItem *item = NULL, *empty = NULL;
    int ret = 0, remSpace = 0;
    bool res = false;
    /* find field */
    ret = mmiFindField(data_id, &item, &empty, &remSpace);
    if(ret != MM_RET_CODE_OK) {
        return ret;
    }

    /* invalidate field */
    res = mmiFlashWrite((uint32_t)item + QWORD_LEN, (uint8_t*)mm_EraseMask, QWORD_LEN);
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
    res = mmiFlashWrite(MEMORY_MANAGER2_OFFSET, (uint8_t*)mm_PageStatePassive, sizeof(mm_PageStatePassive));
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
