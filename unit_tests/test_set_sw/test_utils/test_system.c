#include "test_system.h"

#include <stdio.h>
#include <stdlib.h>

#include "io_utils.h"
#include "data_utils.h"
#include "unit_test_check.h"

static bool test_malloc_zero(void) {
    bool res = false;
    char *ptr = malloc(0);
    if (NULL != ptr) {
        res = false; /*X86_64*/
        free(ptr);
    } else {
        res = true;
    }
    return res;
}

static float byte2Mbyte(float byte){
    float mByte = byte/(1024.0f*1024.0f);
    return mByte;
}

#ifndef X86_64
/*Long test*/
static bool test_heap_set(void) {
#ifdef  X86_64
    printf("\n%s():"CRLF,__FUNCTION__);
#endif
    bool res = false;
    uint32_t byte = 3;
    uint32_t mult = 10;
    uint32_t div = 2;
    while (1) {
        char *ptr = NULL;
        ptr = (char*) malloc(byte);
        if (ptr) {
            memset(ptr, 0x00, byte);
            byte = (byte * mult) / div;
            res = true;
            // print_bytes (byte);
            free(ptr);
        } else {
            break;
        }
    }
#ifdef X86_64
    printf("can malloc %u bytes %u kBytes %f MBytes"CRLF,byte,byte/1024, byte2Mbyte(byte));
    /* PC:  2,094,737,251 byte */
#endif
#ifdef HAS_MCU
    LOG_INFO(SYS,"can malloc %u bytes %u kBytes %f MBytes",byte,byte/1024, byte2Mbyte(byte));
    /* MCU: can malloc 46875 bytes 45,77 kBytes */
#endif
    return res;
}
#endif

bool test_system(void) {
    uint16_t real_size=0;
#ifdef X86_64
    EXPECT_FALSE(test_malloc_zero());
#else
    EXPECT_TRUE(test_malloc_zero());
#endif
#ifndef X86_64
    EXPECT_TRUE(test_heap_set());
#endif
    EXPECT_TRUE( try_alloc_on_stack(10, 0x55,&real_size));
    EXPECT_EQ( 10,real_size);
    return true;
}