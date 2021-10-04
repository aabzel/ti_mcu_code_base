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
        res = false;
        free(ptr);
    } else {
        res = true;
    }
    return res;
}


static bool test_heap_set(void) {
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

    io_printf("can malloc %u bytes"CRLF,byte);

    return res;
}

bool test_system(void) {
    uint16_t real_size=0;
    EXPECT_TRUE(test_malloc_zero());
    EXPECT_TRUE(test_heap_set());
    EXPECT_TRUE( try_alloc_on_stack(10, 0x55,&real_size));
    EXPECT_EQ( 10,real_size);
    return true;
}
