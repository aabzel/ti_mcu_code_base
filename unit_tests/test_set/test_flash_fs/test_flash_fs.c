#include "test_flash_fs.h"

#include <stdbool.h>
#include <stdint.h>

#include "data_utils.h"
#include "flash_fs.h"
#include "io_utils.h"
#include "memory_layout.h"
#include "param_ids.h"
#include "unit_test_check.h"

#ifdef HAS_RNG
#include "rng_drv.h"
#endif /*HAS_RNG*/

/*
tsr flash_fs+
*/

bool test_flash_fs_1(void) {
    EXPECT_EQ(7, sizeof(mmItem_t));
    uint16_t data_id = 0x1234;
    uint16_t write_value = 0xaabb;
    uint16_t read_value = 0;
    uint16_t read_value_len = 0;
    EXPECT_TRUE(mm_set(data_id, (unsigned char*)&write_value, sizeof(write_value)));
    EXPECT_TRUE(mm_get(data_id, (unsigned char*)&read_value, sizeof(read_value), &read_value_len));
    EXPECT_EQ(2, read_value_len);
    EXPECT_EQ(write_value, read_value);
    return true;
}

bool test_flash_fs_inval(void) {
    EXPECT_EQ(7, sizeof(mmItem_t));
    uint16_t data_id = 609;
    uint16_t write_value = 1839;
    uint16_t read_value = 0;
    uint16_t read_value_len = 0;
    EXPECT_TRUE(mm_set(data_id, (unsigned char*)&write_value, sizeof(write_value)));
    EXPECT_TRUE(mm_get(data_id, (unsigned char*)&read_value, sizeof(read_value), &read_value_len));
    EXPECT_EQ(2, read_value_len);
    EXPECT_EQ(write_value, read_value);

    EXPECT_TRUE(mm_invalidate(data_id));
    EXPECT_FALSE(mm_get(data_id, (unsigned char*)&read_value, sizeof(read_value), &read_value_len));
    return true;
}

#ifdef HAS_RNG
bool test_flash_fs_2(void) {
    uint16_t data_id;
    uint16_t write_value;
    uint16_t read_value = 0;
    uint16_t read_value_len = 0;

    for(uint16_t i = PAR_ID_TEST_START; i < PAR_ID_TEST_END; i++) {
        data_id = (uint16_t)(i);
        write_value = (uint16_t)generate_rand_uint32();
        read_value = 0;
        read_value_len = 0;
        EXPECT_TRUE(mm_set(data_id, (unsigned char*)&write_value, sizeof(write_value)));
        EXPECT_TRUE(mm_get(data_id, (unsigned char*)&read_value, sizeof(read_value), &read_value_len));
        EXPECT_EQ(2, read_value_len);
        EXPECT_EQ(write_value, read_value);
    }
    return true;
}

bool test_flash_fs_3(void) {
    uint16_t data_id = PAR_ID_TEST_START + ((uint16_t)generate_rand_uint32() % (PAR_ID_TEST_END - PAR_ID_TEST_START));
    uint16_t write_value = 34;
    uint16_t read_value = 0;
    uint16_t read_value_len = 0;
    EXPECT_TRUE(mm_set(data_id, (unsigned char*)&write_value, sizeof(write_value)));

    EXPECT_TRUE(mm_get(data_id, (unsigned char*)&read_value, sizeof(read_value), &read_value_len));
    EXPECT_EQ(2, read_value_len);
    EXPECT_EQ(write_value, read_value);

    write_value = 35;
    read_value = 0;
    read_value_len = 0;
    EXPECT_TRUE(mm_set(data_id, (unsigned char*)&write_value, sizeof(write_value)));

    EXPECT_TRUE(mm_get(data_id, (unsigned char*)&read_value, sizeof(read_value), &read_value_len));
    EXPECT_EQ(2, read_value_len);
    EXPECT_EQ(write_value, read_value);
    return true;
}
#endif /*HAS_RAND*/

static uint32_t calc_toggle_exp_page(uint32_t page) {
    uint32_t exp_page = 0;
    if(MEMORY_MANAGER1_OFFSET == page) {
        exp_page = MEMORY_MANAGER2_OFFSET;
    } else if(MEMORY_MANAGER2_OFFSET == page) {
        exp_page = MEMORY_MANAGER1_OFFSET;
    }
    return exp_page;
}

bool test_flash_fs_toggle(void) {
    uint32_t read_mm_page_start = 0;
    uint32_t read_mm_page_len = 1;
    uint32_t origin_file_cnt=0;
    uint32_t new_file_cnt=0;
    uint32_t exp_mm_page_start = 0;
    uint32_t spare_cnt=0;

    EXPECT_TRUE(mm_get_active_page(&read_mm_page_start, &read_mm_page_len));
    EXPECT_GR(MEMORY_MANAGER1_LENGTH / 2, read_mm_page_len, 0U);
    origin_file_cnt = mm_cnt_files(read_mm_page_start, read_mm_page_len, &spare_cnt);

    exp_mm_page_start = calc_toggle_exp_page(read_mm_page_start);
    EXPECT_TRUE(mm_turn_page());

    EXPECT_TRUE(mm_get_active_page(&read_mm_page_start, &read_mm_page_len));
    EXPECT_EQ(exp_mm_page_start, read_mm_page_start);
    EXPECT_GR(MEMORY_MANAGER1_LENGTH / 2, read_mm_page_len, 1);

    new_file_cnt= mm_cnt_files(read_mm_page_start, read_mm_page_len, &spare_cnt);
    EXPECT_EQ(new_file_cnt, origin_file_cnt);

    return true;
}
