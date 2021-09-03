#include "test_flash_fs.h"

#include <stdbool.h>
#include <stdint.h>

#include "data_utils.h"
#include "flash_fs.h"
#include "io_utils.h"
//#include "rng.h"
#include "unit_test_check.h"

/*
tsr flash_fs+
*/


bool test_flash_fs_1(void) {
    EXPECT_EQ(8, sizeof(mmItem));
    int ret = -1;
    uint16_t data_id = 0x1234;
    uint16_t write_value = (uint16_t)0xaabb;
    uint16_t read_value = 0;
    uint16_t read_value_len = 0;
    ret = mm_set(data_id, (unsigned char*)&write_value, sizeof(write_value));
    EXPECT_EQ(MM_RET_CODE_OK, ret);
    ret = mm_get(data_id, (unsigned char*)&read_value, sizeof(read_value), &read_value_len);
    EXPECT_EQ(MM_RET_CODE_OK, ret);
    EXPECT_EQ(2, read_value_len);
    EXPECT_EQ(write_value, read_value);
    return true;
}
#ifdef HAS_RAND
bool test_flash_fs_2(void) {
    int ret = -1;
    unsigned short data_id;
    uint16_t write_value;
    uint16_t read_value = 0;
    uint16_t read_value_len = 0;

    for (uint16_t i = 0; i < 5000; i++) {
        data_id = (uint16_t)(i % (5000 / 100));
        write_value = (uint16_t)generate_rand_uint32();
        read_value = 0;
        read_value_len = 0;
        ret = mm_set(data_id, (unsigned char*)&write_value, sizeof(write_value));
        EXPECT_EQ(MM_RET_CODE_OK, ret);
        ret = mm_get(data_id, (unsigned char*)&read_value, sizeof(read_value), &read_value_len);
        EXPECT_EQ(MM_RET_CODE_OK, ret);
        EXPECT_EQ(2, read_value_len);
        EXPECT_EQ(write_value, read_value);
    }
    return true;
}
bool test_flash_fs_3(void) {
    int ret = -1;
    unsigned short data_id = (uint16_t)generate_rand_uint32();
    uint16_t write_value = 34;
    uint16_t read_value = 0;
    uint16_t read_value_len = 0;
    ret = mm_set(data_id, (unsigned char*)&write_value, sizeof(write_value));
    EXPECT_EQ(MM_RET_CODE_OK, ret);

    ret = mm_get(data_id, (unsigned char*)&read_value, sizeof(read_value), &read_value_len);
    EXPECT_EQ(MM_RET_CODE_OK, ret);
    EXPECT_EQ(2, read_value_len);
    EXPECT_EQ(write_value, read_value);

    write_value = 35;
    read_value = 0;
    read_value_len = 0;
    ret = mm_set(data_id, (unsigned char*)&write_value, sizeof(write_value));
    EXPECT_EQ(MM_RET_CODE_OK, ret);

    ret = mm_get(data_id, (unsigned char*)&read_value, sizeof(read_value), &read_value_len);
    EXPECT_EQ(MM_RET_CODE_OK, ret);
    EXPECT_EQ(2, read_value_len);
    EXPECT_EQ(write_value, read_value);
    return true;
}
#endif /*HAS_RAND*/
